// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "log.hpp"
#include "time.hpp"

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <syncstream>

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
		log_stream << "[LOG] [" << Time::GetCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[LOG] [" << Time::GetCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& warn(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cout.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[WARN] [" << Time::GetCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[WARN] [" << Time::GetCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& error(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cerr.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[ERROR] [" << Time::GetCurrentTime() << "] " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		// Log to console
		output_stream << "[ERROR] [" << Time::GetCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		return output_stream;
	}

	std::ostream& fatal(const std::string& message, const std::source_location& location)
	{
		static SingleOutputStream output_stream(std::cerr.rdbuf());
		std::lock_guard<std::mutex> lock(log_mutex);

		// TODO: Add a windows specific means of outputting to OutputDebugString

		// Log to file
		log_stream << "[FATAL] [" << Time::GetCurrentTime() << "] - " << location.file_name() << ":" << location.line()
				   << " - FUNC: " << location.function_name() << " - " << message << std::endl;

		log_stream.flush();

		// Log to console
		output_stream << "[FATAL] [" << Time::GetCurrentTime() << "] - " << location.file_name() << ":" << location.line()
					  << " - FUNC: " << location.function_name() << " - " << message << std::endl;
		output_stream.flush();
		log_file.close();
		return output_stream;
	}


} // namespace gen
