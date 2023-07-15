#pragma once
#include <logger/context.hpp>

namespace gen::refactor::logger {
struct Sink {
	virtual ~Sink() = default;

	virtual void handle(std::string_view formatted, Context const& context) = 0;
};
} // namespace gen::refactor::logger
