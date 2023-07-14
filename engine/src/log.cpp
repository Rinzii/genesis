// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "log.hpp"

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <syncstream>
#include <chrono>

// Tell MSVC to shut up about using std::gmtime
// NOLINTNEXTLINE
#define _CRT_SECURE_NO_WARNINGS

namespace
{
	class SingleStreamBuffer : public std::streambuf
	{
	public:
		explicit SingleStreamBuffer(std::streambuf* buf)
			: buf_(buf)
		{
		}

	protected:
		int overflow(int c) override
		{
			if (c != EOF)
			{
				// Convert the integer value to a character
				const char character = static_cast<char>(c);

				// Check if the buffer is valid
				if (buf_)
					buf_->sputc(character); // Add the character to the buffer
			}
			return c;
		}

		std::streambuf* buf_;
	}; // class SingleStreamBuffer

	class SingleOutputStream : public std::ostream
	{
	public:
		explicit SingleOutputStream(std::streambuf* buf)
			: std::ostream(&singleStreamBuf_), singleStreamBuf_(buf)
		{
		}

	private:
		SingleStreamBuffer singleStreamBuf_;
	}; // class SingleOutputStream

	std::string getCurrentTime()
	{
		// This all SHOULD be thread safe but it needs to be tested further.
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;
		ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%d %X");
		return ss.str();
	}

} // namespace

namespace gen::logger
{

	std::ofstream log_file("genesis.log");
	std::osyncstream log_stream(log_file);
	std::mutex log_mutex;

	std::ostream& trace(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cout.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[TRACE] [" << getCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[TRACE] [" << getCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& log(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cout.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[LOG] [" << getCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[LOG] [" << getCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& warn(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cout.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[WARN] [" << getCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[WARN] [" << getCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& error(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cerr.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[ERROR] [" << getCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[ERROR] [" << getCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& fatal(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cerr.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[FATAL] [" << getCurrentTime() << "] - " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		log_stream.flush();

		// Log to console
		output_stream << "[FATAL] [" << getCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		output_stream.flush();
		log_file.close();
		return output_stream;
	}


} // namespace gen
