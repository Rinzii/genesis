// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "io/file.hpp"
#include <stdexcept>
#include <chrono>


namespace gen {


    bool File::open(const fs::path & filePath)
	{
		std::lock_guard<std::mutex> lock(m_mutex); // Lock the mutex to ensure thread safety

		m_filePath = filePath;

	    std::ofstream createFile(filePath);
	    if (!createFile.is_open())
        {
            return false;
        }

	    m_file.open(filePath, std::ios::in | std::ios::out | std::ios::binary);

	    return m_file.is_open();
    }

	bool File::read(void* data, const std::size_t dataSize, u64& /* [out] */ bytesRead)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_file.is_open() && data != nullptr && dataSize > 0)
		{
			m_file.read(static_cast<char*>(data), static_cast<std::streamsize>(dataSize));
			const std::streamsize readSize = m_file.gcount();
			if (readSize > 0)
			{
				bytesRead = static_cast<std::uint32_t>(readSize);
				return true;
			}
		}
		bytesRead = -1;
		return false;
	}

	bool File::read(const void* data, const std::size_t dataSize, u64& bytesRead)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_file.is_open() && data != nullptr && dataSize > 0)
		{
			m_file.read(reinterpret_cast<char*>(const_cast<void*>(data)), static_cast<std::streamsize>(dataSize));
			const std::streamsize readSize = m_file.gcount();
			if (readSize > 0)
			{
				bytesRead = static_cast<std::uint32_t>(readSize);
				return true;
			}
		}
		bytesRead = -1;
		return false;
	}


	bool File::write(const void * data,
					 const std::size_t dataSize,
					 const std::streampos position,
					 const std::ios_base::openmode mode,
					 const bool flush)
	{
		std::lock_guard<std::mutex> lock(m_mutex); // Lock the mutex to ensure thread safety

		if (m_file.is_open())
		{
			if (position != -1)
			{
				// Seek to the specified position in the file
				m_file.seekp(position);
			}

			// Write the data to the file
			m_file.write(static_cast<const char*>(data), static_cast<std::streamsize>(dataSize));

			if (flush)
			{
				m_file.flush();
			}

			return true;
		}

		return false;
		}

	i64 File::seek(const i64 position) {
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_file.is_open())
		{
			m_file.seekg(position, std::ios::beg);
			m_file.seekp(position, std::ios::beg);

			if (m_file.fail())
			{
				return -1;
			}

			return tell();
		}
		else
		{
			return -1;
		}
	}

	i64 File::tell() {
		std::lock_guard<std::mutex> lock(m_mutex); // Lock the mutex to ensure thread safety

		if (m_file.is_open())
		{
			return static_cast<i64>(m_file.tellg());
		}
		else
		{
			return -1;
		}
	}

	bool File::isValid() const {
		return m_file.is_open();
	}

	i64 File::getFileSize()
	{
		std::lock_guard<std::mutex> lock(m_mutex); // Lock the mutex to ensure thread safety

        if (m_file.is_open())
        {
            m_file.seekg(0, std::ios::end);
			const std::streampos fileSize = m_file.tellg();

            if (fileSize != -1)
            {
                return static_cast<i64>(fileSize);
            }
        }

        return -1; // Return -1 if the file is not valid or if there was an error getting the file size
	}


	i64 File::getFileTimeStamp()
	{
		std::lock_guard<std::mutex> lock(m_mutex); // Lock the mutex to ensure thread safety

		if (m_file.is_open())
		{
            const std::filesystem::path filePath = m_filePath;
			const std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(filePath);
            return std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();

		}

		return -1; // Return -1 if the file is not valid or if there was an error getting the file timestamp
	}




	} // namespace gen
