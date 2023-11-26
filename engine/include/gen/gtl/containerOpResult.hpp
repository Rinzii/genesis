// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <algorithm>

namespace gtl
{
	template <class Iterator>
	class ContainerOpResult
	{
	public:
		using iterator_type = Iterator;

		inline ContainerOpResult();

		inline ContainerOpResult( const ContainerOpResult & other );

		inline ContainerOpResult( ContainerOpResult && other ) noexcept;

		template <class OtherIterator>
		inline explicit ContainerOpResult( const ContainerOpResult<OtherIterator> & other );

		inline ContainerOpResult & operator=( const ContainerOpResult & other );

		inline ContainerOpResult & operator=( ContainerOpResult && other ) noexcept;

		template <class OtherIterator>
		inline ContainerOpResult & operator=( const ContainerOpResult<OtherIterator> & other );

		void swap( ContainerOpResult & other ) noexcept;

		inline ~ContainerOpResult() = default;

		[[nodiscard]] inline bool IsSuccessful() const noexcept { return m_success; }

		inline iterator_type GetIterator() const noexcept { return m_iterator; }

		static inline ContainerOpResult Success( iterator_type iter ) { return ContainerOpResult{ iter, true }; }

		static inline ContainerOpResult Success() { return ContainerOpResult{ iterator_type{}, true }; }

		static inline ContainerOpResult Failure( iterator_type iter ) { return ContainerOpResult{ iter, false }; }

		static inline ContainerOpResult Failure() { return ContainerOpResult{ iterator_type{}, false }; }


	private:
		inline ContainerOpResult( iterator_type iter, bool success );

		iterator_type m_iterator;
		bool m_success;
	};

	template <class Iterator>
	void ContainerOpResult<Iterator>::swap(ContainerOpResult & other) noexcept
	{
		std::swap(m_iterator, other.m_iterator);
		std::swap(m_success, other.m_success);
	}

	template <class Iterator>
	inline ContainerOpResult<Iterator>::ContainerOpResult() : m_iterator{}, m_success{ false }
	{
	}

	template <class Iterator>
	inline ContainerOpResult<Iterator>::ContainerOpResult( const ContainerOpResult & other ) : m_iterator{ other.m_iterator }, m_success{ other.m_success }
	{
	}

	template <class Iterator>
	inline ContainerOpResult<Iterator>::ContainerOpResult( ContainerOpResult && other ) noexcept : m_iterator{ std::move( other.m_iterator ) }, m_success{ std::move( other.m_success ) }
	{
	}

	template <class Iterator>
	template <class OtherIterator>
	inline ContainerOpResult<Iterator>::ContainerOpResult( const ContainerOpResult<OtherIterator> & other ) : m_iterator{ other.GetIterator() }, m_success{ other.IsSuccessful() }
	{
	}

	template <class Iterator>
	inline ContainerOpResult<Iterator> & ContainerOpResult<Iterator>::operator=( const ContainerOpResult & other )
	{
		ContainerOpResult( other ).swap( *this );
		return *this;
	}

	template <class Iterator>
	inline ContainerOpResult<Iterator> & ContainerOpResult<Iterator>::operator=( ContainerOpResult && other ) noexcept
	{
		ContainerOpResult( std::move( other ) ).swap( *this );
		return *this;
	}

	template <class Iterator>
	template <class OtherIterator>
	inline ContainerOpResult<Iterator> & ContainerOpResult<Iterator>::operator=( const ContainerOpResult<OtherIterator> & other )
	{
		ContainerOpResult( other ).swap( *this );
		return *this;
	}

	template <class Iterator>
	inline ContainerOpResult<Iterator>::ContainerOpResult( iterator_type iter, bool success ) : m_iterator{ iter }, m_success{ success }
	{
	}

	template <class Iterator>
	inline bool operator==( const ContainerOpResult<Iterator> & lhs, const ContainerOpResult<Iterator> & rhs )
	{
		return lhs.IsSuccessful() == rhs.IsSuccessful() && lhs.GetIterator() == rhs.GetIterator();
	}

	template <class Iterator>
	inline bool operator!=( const ContainerOpResult<Iterator> & lhs, const ContainerOpResult<Iterator> & rhs )
	{
		return !( lhs == rhs );
	}




} // namespace gen