#pragma once
#include <logger/config.hpp>
#include <logger/sink.hpp>
#include <logger/target.hpp>
#include <memory>
#include <stdexcept>

namespace gen::refactor::logger {
class Instance {
public:
	struct DuplicateError : std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	Instance& operator=(Instance&&) = delete;

	explicit Instance(char const* filePath, Config config = {});
	~Instance();

	[[nodiscard]] auto getConfig() const -> Config;
	auto setConfig(Config config) -> void;

	auto addSink(std::unique_ptr<Sink> sink) -> void;

	static auto print(std::string_view message, Context const& context) -> void;

private:
	struct Impl;
	struct Deleter {
		auto operator()(Impl const* ptr) const -> void;
	};

	// NOLINTNEXTLINE
	inline static Impl* s_instance{};
	std::unique_ptr<Impl, Deleter> m_impl{};
};
} // namespace gen::refactor::logger
