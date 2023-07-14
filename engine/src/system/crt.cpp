// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "system/crt.hpp"

namespace gen
{
    void memcpy(void* dest, const void* source, std::size_t size)
    {
        std::memcpy(dest, source, size);
    }

    void memmove(void* dest, const void* source, std::size_t size)
    {
        std::memmove(dest, source, size);
    }

    void memset(void* buffer, std::int32_t value, std::size_t size)
    {
        std::memset(buffer, value, size);
    }

	void memzero(void* buffer, std::size_t size )
    {
        std::memset(buffer, 0, size);
    }

	int memcmp(const void* a, const void* b, std::size_t size)
    {
        return std::memcmp(a, b, size);
    }

	std::size_t strlen( const char * str )
	{
        return std::strlen( str );
    }

	std::size_t strlen( const wchar_t * str )
	{
        return std::wcslen( str );
    }

#if defined(GEN_PLATFORM_LINUX) || defined(GEN_PLATFORM_APPLE)
    std::size_t strlen( const char * str, std::size_t maxBufferSize )
    {
        return ::strnlen( str, maxBufferSize );
    }

    std::size_t strlen( const wchar_t * str, std::size_t maxBufferSize )
    {
        return ::wcsnlen( str, maxBufferSize );
    }
	
	bool strcpy( char* dest, const char* source, std::size_t destSize, std::size_t sourceToCopy )
    {
        std::size_t sourceLength = strlen( source, sourceToCopy );
		const std::size_t n = destSize < ( sourceLength + 1 ) ? destSize : ( sourceLength + 1 );
		std::strncpy( dest, source, n );
		if (n > 0)
		{
			dest[n - 1] = '\0';
		}
		return true;
    }
	
	bool strcpy( wchar_t* dest, const wchar_t* source, std::size_t destSize, std::size_t sourceToCopy )
    {
        std::size_t sourceLength = strlen( source, sourceToCopy );
		const std::size_t n = destSize < ( sourceLength + 1 ) ? destSize : ( sourceLength + 1 );
		std::wcsncpy( dest, source, n );
		if (n > 0)
        {
            dest[n - 1] = '\0';
        }
		return true;
	}
	
	bool strcat( char* dest, const char* source, std::size_t destSize, std::size_t sourceToCopy )
    {
        std::size_t sourceLength = strlen( source, sourceToCopy );
		// strncat writes n chars and then appends null
		const std::size_t nbytes = destSize < ( sourceLength + 1 ) ? destSize : ( sourceLength + 1 );
		if ( nbytes < 1 )
		{
			*dest = '\0';
			return true;
		}
		std::strncat( dest, source, nbytes - 1 );
		return true;
    }
	
	bool strcat( wchar_t* dest, const wchar_t* source, std::size_t destSize, std::size_t sourceToCopy ) {
        std::size_t sourceLength = strlen(source, sourceToCopy);
        // wcsncat writes n chars and then appends null
        const std::size_t nbytes = destSize < (sourceLength + 1) ? destSize : (sourceLength + 1);
        if (nbytes < 1) {
            *dest = '\0';
            return true;
        }
        std::wcsncat(dest, source, nbytes - 1);
        return true;
    }
	
#else
	size_t strlen( const char* str, std::size_t maxBufferSize )
	{
		return ::strnlen_s( str, maxBufferSize );
	}
	
	size_t strlen( const wchar_t* str, std::size_t maxBufferSize )
	{
		return ::wcsnlen_s( str, maxBufferSize );
	}

	bool strcpy( char* dest, const char* source, std::size_t destSize, std::size_t sourceToCopy )
	{
	#ifdef GEN_COMPILER_CLANG
		return ::strncpy_s( dest, destSize, source, sourceToCopy < RSIZE_MAX ? sourceToCopy : RSIZE_MAX ) == 0;
	#else
		return ::strncpy_s( dest, destSize, source, sourceToCopy ) == 0;
	#endif
	}

	bool strcpy( wchar_t* dest, const wchar_t* source, std::size_t destSize, std::size_t sourceToCopy )
	{
	#ifdef GEN_COMPILER_CLANG
		return ::wcsncpy_s( dest, destSize, source, sourceToCopy < RSIZE_MAX ? sourceToCopy : RSIZE_MAX ) == 0;
	#else
		return ::wcsncpy_s( dest, destSize, source, sourceToCopy ) == 0;
	#endif
	}

	bool strcat( char* dest, const char* source, std::size_t destSize, std::size_t sourceToCopy )
	{
	#ifdef GEN_COMPILER_CLANG
		return ::strncat_s( dest, destSize, source, sourceToCopy < RSIZE_MAX ? sourceToCopy : RSIZE_MAX ) == 0;
	#else
		return ::strncat_s( dest, destSize, source, sourceToCopy ) == 0;
	#endif
	}

	bool strcat( wchar_t* dest, const wchar_t* source, std::size_t destSize, std::size_t sourceToCopy  )
	{
	#ifdef GEN_COMPILER_CLANG
		return ::wcsncat_s( dest, destSize, source, sourceToCopy < RSIZE_MAX ? sourceToCopy : RSIZE_MAX ) == 0;
	#else
		return ::wcsncat_s( dest, destSize, source, sourceToCopy ) == 0;
	#endif
	}

#endif

	std::int32_t strcmp( const char* a, const char* b )
    {
        return std::strcmp( a, b );
    }

	std::int32_t strcmp( const wchar_t* a, const wchar_t* b )
    {
        return std::wcscmp( a, b );
    }

    std::int32_t strcmp( const char* a, const char* b, std::size_t max )
    {
        return std::strncmp( a, b, max );
    }

	std::int32_t strcmp( const wchar_t* a, const wchar_t* b, std::size_t max )
    {
        return std::wcsncmp( a, b, max );
    }


}
