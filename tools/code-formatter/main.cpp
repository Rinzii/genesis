#include <algorithm>
#include <cassert>
#include <filesystem>
#include <format>
#include <iostream>
#include <mutex>
#include <span>
#include <vector>

namespace fs = std::filesystem;

namespace {
struct Options {
	struct ParseError : std::runtime_error {
		using std::runtime_error::runtime_error;
	};
	struct Usage {};

	bool quiet{};
	bool simulate{};
	std::string_view rootDir{"."};

	static ParseError unexpected_arg(std::string_view const arg) { return ParseError{std::format("unexpected argument: '{}'", arg)}; }
	static ParseError unrecognized_opt(std::string_view const opt) { return ParseError{std::format("unrecognized option: '{}'", opt)}; }

	static std::string buildUsage(std::string_view const appName) { return std::format("usage: {} [-q|--quiet] [-s|--simulate] [path]", appName); }

	void parse(std::span<char const* const> args) {
		auto unmatched = std::vector<std::string_view>{};
		for (std::string_view const arg : args) {
			if (arg.starts_with("--")) {
				option(arg.substr(2));
			} else if (arg.starts_with('-')) {
				options(arg.substr(1));
			} else {
				unmatched.push_back(arg);
			}
		}

		if (!unmatched.empty()) {
			if (unmatched.size() > 1) { throw unexpected_arg(unmatched.front()); }
			rootDir = unmatched.front();
		}
	}

	void option(std::string_view const arg) {
		if (arg == "quiet") {
			quiet = true;
			return;
		}

		if (arg == "simulate") {
			simulate = true;
			return;
		}

		if (arg == "usage" || arg == "help") { throw Usage{}; }

		throw unrecognized_opt(arg);
	}

	void options(std::span<char const> opts) {
		for (char const opt : opts) {
			switch (opt) {
			case 'q': quiet = true; break;
			case 's': simulate = true; break;
			default: throw unrecognized_opt({&opt, 1});
			}
		}
	}
};

struct App {
	static constexpr std::string_view command{"clang-format"};
	static constexpr std::string_view null_v {
#if defined(_WIN32)
		"NUL"
#else
		"/dev/null"
#endif
	};

	[[nodiscard]] static bool available() {
		// NOLINTNEXTLINE
		return std::system(std::format("{} --version > {}", command, null_v).c_str()) == 0;
	}

	static bool format(std::string_view const path) {
		if (!fs::is_regular_file(path)) { return false; }
		// NOLINTNEXTLINE
		return std::system(std::format("{} -i {}", command, path).c_str()) == 0;
	}

	std::vector<std::string_view> ignores{"build/", "out/", "ext/", "cmake/"};
	std::vector<std::string_view> extensions{".hpp", ".cpp"};

	[[nodiscard]] bool shouldIgnore(std::string_view const path) const {
		return std::ranges::any_of(ignores, [path](std::string_view const ignore) { return path.find(ignore) != std::string_view::npos; });
	}

	[[nodiscard]] bool shouldFormat(fs::path const& path) const {
		if (shouldIgnore(path.string())) { return false; }
		return std::ranges::any_of(extensions, [ext = path.extension()](std::string_view const extension) { return ext == extension; });
	}

	void run(Options const& options) const {
		if (!available()) { throw std::runtime_error{std::format("{} not available", command)}; }

		if (options.simulate && !options.quiet) { std::cout << "-- simulate enabled, source files will not be modified\n"; }

		auto count{0};
		if (!options.quiet) { std::cout << std::format("-- formatting source files in '{}' :\n\n", options.rootDir); }
		for (auto const& itr : fs::recursive_directory_iterator{options.rootDir}) {
			if (auto const& path = itr.path(); shouldFormat(path)) {
				auto const pathString = path.generic_string();
				if (!options.quiet) { std::cout << std::format("{}\n", pathString); }
				if (!options.simulate) {
					if (format(pathString)) { ++count; }
				} else {
					++count;
				}
			}
		}
		if (!options.quiet) { std::cout << "\n"; }
		std::cout << std::format(" == {} source files formatted\n", count);
	}
};
} // namespace

int main(int argc, char** argv) {
	assert(argc > 0);
	auto const usage = Options::buildUsage(fs::path{*argv}.filename().string());
	auto const args = std::span{argv, static_cast<std::size_t>(argc)}.subspan(1);
	auto options = Options{};
	try {
		options.parse(args);

		if (!fs::is_directory(options.rootDir)) {
			std::cerr << std::format("failed to resolve root directory '{}'\n", options.rootDir);
			return EXIT_FAILURE;
		}

		App{}.run(options);

	} catch (Options::ParseError const& error) {
		std::cerr << std::format("{}\n{}\n", error.what(), usage);
		return EXIT_FAILURE;
	} catch (Options::Usage) {
		std::cout << std::format("{}\n", usage);
		return EXIT_SUCCESS;
	} catch (std::exception const& e) {
		std::cerr << std::format("fatal error: {}\n", e.what());
		return EXIT_FAILURE;
	}
}
