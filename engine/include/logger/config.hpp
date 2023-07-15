#pragma once
#include <logger/level.hpp>
#include <logger/target.hpp>
#include <array>
#include <cassert>
#include <string>
#include <unordered_map>

namespace gen::refactor::logger {
enum class Timestamp { eLocal, eGm };
enum class Location { eFilename, eFullPath };

struct Config {
	static constexpr std::string_view default_format_v{"[{level}][T{thread}] {message} [{timestamp}] [{location}]"};

	struct Category {
		std::string_view id{};
		Level maxLevel{};
	};

	struct Output {
		std::array<Target, static_cast<std::size_t>(Level::eCOUNT_)> map{};

		constexpr Target operator()(Level const level) const {
			assert(level < Level::eCOUNT_);
			return map[static_cast<std::size_t>(level)];
		}
	};

	Output output{};
	std::string format{default_format_v};
	Level maxLevel{Level::eDebug};
	std::unordered_map<std::string_view, Level> categoryMaxLevels{};
	std::unordered_map<Level, Target> levelTargets{};
	Timestamp timestamp{Timestamp::eLocal};
	Location location{Location::eFilename};
};
} // namespace gen::refactor::logger
