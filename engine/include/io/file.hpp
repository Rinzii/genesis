/**
 * @file file.hpp
 * @brief Defines the Thread-safe Synchronous File class.
 *
 * This header provides the declaration of the Thread-safe Synchronous File class, which is designed
 * for safe file handling in a multi-threaded environment.
 */

#pragma once

#include "core.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>

namespace gen {

namespace fs = std::filesystem;

/**
 * @class File
 * @brief Thread-safe Synchronous File class.
 *
 * The File class provides a thread-safe mechanism for synchronous file handling.
 * It supports opening, reading, writing, seeking, and retrieving information about files.
 */
class File {
public:
	/**
	 * @brief Enumeration of file modes.
	 */
	enum Mode : int {
		in = std::ios::in,		   //!< Read mode.
		out = std::ios::out,	   //!< Write mode.
		ate = std::ios::ate,	   //!< Open at the end of the file.
		app = std::ios::app,	   //!< Append mode.
		trunc = std::ios::trunc,   //!< Truncate the file if it exists.
		binary = std::ios::binary, //!< Binary mode.
	};

	/**
	 * @brief Default constructor.
	 */
	File() = default;

	/**
	 * @brief Default destructor.
	 */
	~File() = default;

	// Disable copy and assignment
	File(File const&) = delete;
	File& operator=(File const&) = delete;

	/**
	 * @brief Opens a file with the specified file path.
	 * @param filePath The path to the file.
	 * @return `true` if the file is successfully opened, `false` otherwise.
	 */
	bool open(fs::path const& filePath);

	/**
	 * @brief Reads data from the file into the provided buffer.
	 * @param data Pointer to the buffer where the read data will be stored.
	 * @param dataSize The size of the data to read.
	 * @param bytesRead [out] The number of bytes actually read from the file.
	 * @return `true` if the read operation is successful, `false` otherwise.
	 */
	bool read(void* data, std::size_t dataSize, u64& /* [out] */ bytesRead);

	/**
	 * @brief Reads data from the file into the provided buffer.
	 * @param data Pointer to the buffer where the read data will be stored.
	 * @param dataSize The size of the data to read.
	 * @param bytesRead [out] The number of bytes actually read from the file.
	 * @return `true` if the read operation is successful, `false` otherwise.
	 */
	bool read(void const* data, std::size_t dataSize, u64& /* [out] */ bytesRead);

	/**
	 * @brief Writes data to the file.
	 * @param data Pointer to the data to be written.
	 * @param dataSize The size of the data to write.
	 * @param position The position in the file where the data will be written.
	 *        If set to -1, the data will be appended to the end of the file.
	 * @param mode The file mode for opening the file.
	 * @param flush Whether to flush the file after writing.
	 * @return `true` if the write operation is successful, `false` otherwise.
	 */
	bool write(void const* data, std::size_t dataSize, std::streampos position = -1, std::ios_base::openmode mode = std::ios::app, bool flush = false);

	/**
	 * @brief Seeks to the specified position in the file.
	 * @param position The position to seek to.
	 * @return The new position in the file after seeking, or -1 if an error occurs.
	 */
	i64 seek(i64 position);

	/**
	 * @brief Returns the current position in the file.
	 * @return The current position in the file, or -1 if an error occurs.
	 */
	i64 tell();

	/**
	 * @brief Checks if the file is valid and open.
	 * @return `true` if the file is valid and open, `false` otherwise.
	 */
	GEN_NODISCARD bool isValid() const;

	/**
	 * @brief Returns the size of the file.
	 * @return The size of the file in bytes, or -1 if the file is not valid or an error occurs.
	 */
	i64 getFileSize();

	/**
	 * @brief Returns the timestamp of the file.
	 * @return The timestamp of the file as the number of seconds since the epoch,
	 *         or -1 if the file is not valid or an error occurs.
	 */
	i64 getFileTimeStamp();

private:
	std::fstream m_file; //!< The file stream.
	std::mutex m_mutex;	 //!< Mutex for thread safety.
	fs::path m_filePath; //!< The path to the file.
};

} // namespace gen
