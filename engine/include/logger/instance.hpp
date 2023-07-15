#pragma once
#include <logger/config.hpp>
#include <logger/sink.hpp>
#include <logger/target.hpp>
#include <memory>
#include <stdexcept>

namespace gen::refactor::logger {
///
/// \brief Logger Instance: a single instance must be created within main's scope.
///
class Instance {
public:
	///
	/// \brief Error thrown if more than one instance is attempted to be created.
	///
	struct DuplicateError : std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	Instance& operator=(Instance&&) = delete;

	///
	/// \brief Create a logger Instance.
	/// \param filePath path to create/overwrite log file at.
	/// \param config Config to use.
	///
	explicit Instance(char const* filePath, Config config = {});
	~Instance();

	///
	/// \brief Obtain a copy of the Config in use.
	///
	[[nodiscard]] auto getConfig() const -> Config;
	///
	/// \brief Overwrite the Config in use.
	///
	auto setConfig(Config config) -> void;

	///
	/// \brief Add a custom sink.
	///
	auto addSink(std::unique_ptr<Sink> sink) -> void;

	///
	/// \brief Entrypoint for logging (free) functions.
	///
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
