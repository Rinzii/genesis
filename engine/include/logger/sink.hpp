#pragma once
#include <logger/context.hpp>

namespace gen::logger {
///
/// \brief Customizable log sink.
///
struct Sink {
	virtual ~Sink() = default;

	///
	/// \brief Customization point for intercepting logs.
	/// \param formatted Formatted log string.
	/// \param context Log context.
	///
	virtual void handle(std::string_view formatted, Context const& context) = 0;
};
} // namespace gen::logger
