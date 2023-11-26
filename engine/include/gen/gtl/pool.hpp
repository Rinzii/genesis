// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <cstdint>

namespace gtl
{
	class Pool
	{
	public:
		constexpr Pool();

		constexpr Pool(const Pool & other)  = default;

		constexpr Pool(Pool && other);

		constexpr Pool & operator=(const Pool & other)  = default;

		constexpr Pool & operator=(Pool && other);

		constexpr ~Pool()  = default;
		
		void swap(Pool & other);

		inline void * data()  { return m_buffer; }

		inline const void * data() const  { return m_buffer; }

		[[nodiscard]] constexpr std::uint32_t capacity() const  { return m_capacity; }

		[[nodiscard]] constexpr std::uint32_t blockSize() const  { return m_blockSize; }

		[[nodiscard]] constexpr std::uint32_t dataCapacity() const  { return m_capacity * m_blockSize; }

		[[nodiscard]] constexpr bool isMaxCapacity() const  { return m_firstFreeBlockIndex == std::numeric_limits<std::uint32_t>::max(); }

		void clear();
		
		
		///
		/// \brief Initializes the pool with a provided buffer.
		///
		/// \note The user is responsible for releasing the provided buffer when its not using the pool.
		///
		/// \param buffer Pointer to the buffer.
		/// \param bufferSize Size of the buffer.
		/// \param objectSize Size of the object to be stored in the pool.
		///
		void create( void * buffer, std::uint32_t bufferSize, std::uint32_t objectSize );

		void destroy();

		constexpr bool isCreated() const  { return m_buffer != nullptr; }

		void * allocate();

		std::uint32_t allocateIndex() ;

		void free( const void * block );

		void freeIndex( std::uint32_t blockIndex );
		
		inline void * getBlock( std::uint32_t blockIndex );
		
		inline const void * getBlock( std::uint32_t blockIndex ) const;
		
		inline std::uint32_t getBlockIndex( const void * block ) const;
		
		std::uint32_t getFreeBlocksCount() const;
		
		template <class T>
		void upsize( void * buffer, std::uint32_t newCapacity );

	private:

		struct Block
		{
			std::uint32_t m_next;
		};

		void * m_buffer;
		std::uint32_t m_capacity;
		std::uint32_t m_blockSize;
		std::uint32_t m_firstFreeBlockIndex;
		std::uint32_t m_numOfAllocatedBlocks;

		void updateNextFreeBlock( const Block * allocatedBlock ) ;
	};
}

inline void * operator new( std::size_t /*unused*/, gtl::Pool & pool ) 
{
	return pool.allocate();
}