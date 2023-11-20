// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <logger/level.hpp>
#include <logger/target.hpp>
#include <format>
#include <string_view>

namespace gen
{
	namespace logger
	{
		void print(Level level, std::string_view category, std::string_view message);
		void print(Level level, std::string_view category, std::string_view function, std::string_view filePath, int curLine, std::string_view message);
	} // namespace logger

	class Logger
	{
	public:
		using Level = logger::Level;

		explicit Logger(std::string_view category);

		template <typename... Args>
		void error(std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eError, m_category, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void verbose_error(std::string_view function, std::string_view filePath, int curLine, std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eError, m_category, function, filePath, curLine, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void warn(std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eWarn, m_category, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void verbose_warn(std::string_view function, std::string_view filePath, int curLine, std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eWarn, m_category, function, filePath, curLine, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void info(std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eInfo, m_category, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void verbose_info(std::string_view function, std::string_view filePath, int curLine, std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eInfo, m_category, function, filePath, curLine, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void log(std::format_string<Args...> fmt, Args &&... args) const
		{
			info(fmt, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void verbose_log(std::string_view function, std::string_view filePath, int curLine, std::format_string<Args...> fmt, Args &&... args) const
		{
			verbose_info(function, filePath, curLine, fmt, std::forward<Args>(args)...);
		}

		template <typename... Args>
		void debug(std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eDebug, m_category, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void verbose_debug(std::string_view function, std::string_view filePath, int curLine, std::format_string<Args...> fmt, Args &&... args) const
		{
			logger::print(Level::eDebug, m_category, function, filePath, curLine, std::format(fmt, std::forward<Args>(args)...));
		}

	private:
		std::string_view m_category{};
	};

	namespace logger
	{
		inline Logger const general{"general"};
	} // namespace logger
} // namespace gen

// NOLINTBEGIN
#define INTERNAL_GEN_LOG(logger, level, message, ...)                                                                                                          \
	do {                                                                                                                                                       \
		logger.verbose_##level(__FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);                                                                      \
	} while ((void)0, 0)

#define GEN_LOG(logger, message, ...)		INTERNAL_GEN_LOG(logger, log, message, ##__VA_ARGS__)
#define GEN_LOG_ERROR(logger, message, ...) INTERNAL_GEN_LOG(logger, error, message, ##__VA_ARGS__)
#define GEN_LOG_WARN(logger, message, ...)	INTERNAL_GEN_LOG(logger, warn, message, ##__VA_ARGS__)
#define GEN_LOG_INFO(logger, message, ...)	INTERNAL_GEN_LOG(logger, info, message, ##__VA_ARGS__)
#define GEN_LOG_DEBUG(logger, message, ...) INTERNAL_GEN_LOG(logger, debug, message, ##__VA_ARGS__)
// NOLINTEND
