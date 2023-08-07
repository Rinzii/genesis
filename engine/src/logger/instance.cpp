#include <logger/instance.hpp>
#include <algorithm>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#if defined(_WIN32)
	#include <Windows.h>
#endif

namespace gen::logger
{
	namespace
	{
		namespace fs = std::filesystem;

		void append_timestamp(std::string & out, Clock::time_point const & timestamp, Timestamp const mode)
		{
			static auto s_mutex{std::mutex{}};
			static constexpr std::size_t buf_size_v{64};
			auto buffer		= std::array<char, buf_size_v>{};
			auto const time = Clock::to_time_t(timestamp);
			// std::localtime / std::gmtime are not thread safe. cppref:
			//  pointer to a static internal std::tm object on success, or null pointer otherwise.
			// The structure may be shared between std::gmtime, std::localtime, and std::ctime,
			// and may be overwritten on each invocation.
			auto lock = std::unique_lock{s_mutex};
			// NOLINTNEXTLINE
			auto const tm_struct = mode == Timestamp::eUtc ? *std::gmtime(&time) : *std::localtime(&time);
			lock.unlock();
			std::strftime(buffer.data(), buffer.size(), "%F %T", &tm_struct);
			out.append(buffer.data());
		}

		void append_location(std::string & out, std::source_location const & location, Location const mode)
		{
			auto const path = [&]
			{
				auto ret = fs::path{location.file_name()};
				if (mode == Location::eFilename) { ret = ret.filename(); }
				return ret.generic_string();
			}();
			out.append(path);
		}

		struct Formatter
		{
			static constexpr auto open_v{'{'};
			static constexpr auto close_v{'}'};

			// snapshot of formatting-related Config (at the time of Formatter construction).
			// we make copies instead of referring to Config to avoid mutex locks,
			// since users can change the active config at any time.
			struct Data
			{
				// default format string size is 57, which will cause std::string to heap allocate.
				// thus we use a fixed capacity stack string instead.
				FixedString<Config::format_size_v> format{};
				Location location{};
				Timestamp timestamp{};
			};

			// NOLINTNEXTLINE
			Data const & data;
			// message and context don't need to be copied, they are unique per call stack.
			std::string_view message;
			// NOLINTNEXTLINE
			Context const & context;

			// output string (formatted)
			std::string out{};
			// remaining input
			std::string_view format{data.format};
			// current character (preceding format)
			char current{};

			[[nodiscard]] bool at_end() const { return format.empty(); }

			bool advance()
			{
				if (at_end())
				{
					// reset current
					current = {};
					return false;
				}
				current = format.front();
				format	= format.substr(1);
				return true;
			}

			bool try_keyword()
			{
				assert(current == open_v);
				auto const close = format.find_first_of(close_v);
				if (close == std::string_view::npos) { return false; }

				auto const key = format.substr(0, close);
				if (!keyword(key)) { return false; }

				// advance to beyond }
				format = format.substr(close + 1);
				return true;
			}

			bool keyword(std::string_view key)
			{
				if (key == "level")
				{
					out += levelChar(context.level);
					return true;
				}

				if (key == "thread")
				{
					std::format_to(std::back_inserter(out), "{}", static_cast<int>(context.thread));
					return true;
				}

				if (key == "message")
				{
					out.append(message);
					return true;
				}

				if (key == "timestamp")
				{
					append_timestamp(out, context.timestamp, data.timestamp);
					return true;
				}

				if (key == "location")
				{
					append_location(out, context.location, data.location);
					return true;
				}

				return false;
			}

			std::string operator()()
			{
				static constexpr std::size_t reserve_v{128};
				out.reserve(message.size() + reserve_v);
				while (advance())
				{
					if (current == open_v && try_keyword()) { continue; }
					out += current;
				}
				out.append("\n");
				return std::move(out);
			}
		};
	} // namespace

	namespace
	{
		struct ConsoleSink : Sink
		{
			void handle(std::string_view const formatted, Context const & context) final
			{
				// pick stdout / stderr based on level
				auto & stream = context.level == Level::eError ? std::cerr : std::cout;
				// thread-safe: operator<< is atomic
				stream << formatted;
#if defined(_WIN32)
				// assuming null terminated string (which it is)
				OutputDebugStringA(formatted.data());
#endif
			}
		};

		struct FileSink : Sink
		{
			std::string path{};
			std::mutex mutex{};
			std::string buffer{};

			// cv must outlive thread (so it receives the stop signal)
			std::condition_variable_any cv{};
			// thread must be destroyed before cv (so it can signal stop)
			std::jthread thread{};

			FileSink(std::string file_path) : path(std::move(file_path)), thread([this](std::stop_token const & stop) { run(stop); }) {}

			void run(std::stop_token const & stop)
			{
				// remove existing log file
				if (fs::exists(path)) { fs::remove(path); }
				// loop until stopped
				while (!stop.stop_requested())
				{
					auto lock = std::unique_lock{mutex};
					// sleep until buffer is not empty
					cv.wait(lock, stop, [this] { return !buffer.empty(); });
					if (buffer.empty()) { continue; }
					// drain buffer into file, even if stop requested
					if (auto file = std::ofstream{path, std::ios::binary | std::ios::app})
					{
						file << buffer;
						buffer.clear();
					}
				}
			}

			void handle(std::string_view const formatted, [[maybe_unused]] Context const & context) final
			{
				auto lock = std::unique_lock{mutex};
				// append to the existing buffer
				buffer.append(formatted);
				lock.unlock();
				// notify writing thread
				cv.notify_one();
			}
		};
	} // namespace

	struct Instance::Impl
	{
		std::vector<std::unique_ptr<Sink>> sinks{};
		Config config{};
		std::mutex mutex{};

		ConsoleSink console{};
		FileSink file;

		static char const * nonEmptyFilePath(char const * input)
		{
			if (input == nullptr || *input == 0) { return "genesis.log"; }
			return input;
		}

		Impl(char const * filePath) : file(nonEmptyFilePath(filePath)) {}

		void print(std::string_view const message, Context const & context)
		{
			// config is shared state, must synchronize access
			auto lock = std::unique_lock{mutex};
			if (auto const itr = config.categoryMaxLevels.find(context.category); itr != config.categoryMaxLevels.end())
			{
				if (context.level > itr->second) { return; }
			}
			else if (context.level > config.maxLevel) { return; }
			auto const target = [&]
			{
				if (auto const itr = config.levelTargets.find(context.level); itr != config.levelTargets.end()) { return itr->second; }
				return all_v;
			}();

			auto const data = Formatter::Data{.format = config.format, .location = config.location, .timestamp = config.timestamp};
			// cache this for later use
			auto const sinks_empty = sinks.empty();
			// config access complete, release lock
			lock.unlock();

			auto const formatted = Formatter{.data = data, .message = message, .context = context}();

			// console has no state, no sync required
			if ((target & console_v) == console_v) { console.handle(formatted, context); }
			// file maintains its own sync
			if ((target & file_v) == file_v) { file.handle(formatted, context); }

			if ((target & sinks_v) == sinks_v && !sinks_empty)
			{
				// sinks are also shared state, obtain lock again
				// TOCTOU slip throughs are possible due to sinks_empty being stale by this point,
				// but we get to avoid the redundant lock entirely when there are no sinks.
				lock.lock();
				// the game is expected to have zero or one sinks (for Dear ImGui), so we just invoke sink->handle under the lock.
				for (auto const & sink : sinks) { sink->handle(formatted, context); }
			}
		}
	};

	void Instance::Deleter::operator()(Impl const * ptr) const
	{
		// NOLINTNEXTLINE
		delete ptr;
	}

	Instance::Instance(char const * filePath, Config config) : m_impl(new Impl{filePath})
	{
		if (s_instance != nullptr) { throw DuplicateError{"Duplicate logger Instance"}; }
		s_instance = m_impl.get();

		m_impl->config = std::move(config);

		m_impl->print(std::format("logging to file: {}", filePath), Context::make("logger", Level::eInfo));
	}

	Instance::~Instance()
	{
		s_instance = {};
	}

	Config Instance::getConfig() const
	{
		assert(m_impl);
		auto lock = std::scoped_lock{m_impl->mutex};
		// make the copy under mutex lock
		return m_impl->config;
	}

	void Instance::setConfig(Config config)
	{
		assert(m_impl);
		auto lock	   = std::scoped_lock{m_impl->mutex};
		m_impl->config = std::move(config);
	}

	void Instance::addSink(std::unique_ptr<Sink> sink)
	{
		if (!sink) { return; }
		assert(m_impl != nullptr);
		auto lock = std::scoped_lock{m_impl->mutex};
		m_impl->sinks.push_back(std::move(sink));
	}

	void Instance::print(std::string_view const message, Context const & context)
	{
		if (s_instance == nullptr) { return; }
		s_instance->print(message, context);
	}
} // namespace gen::logger
