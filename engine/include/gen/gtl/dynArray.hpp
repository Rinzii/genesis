// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

namespace gtl
{
	template <class T, std::size_t N, class Allocator = std::allocator<T>>
	class alignas(sizeof(T) * N) DynArray
	{
	public:
		using value_type			 = T;
		using allocator_type		 = Allocator;
		using size_type				 = std::size_t;
		using difference_type		 = std::ptrdiff_t;
		using reference				 = value_type &;
		using const_reference		 = const value_type &;
		using pointer				 = typename std::allocator_traits<Allocator>::pointer;
		using const_pointer			 = typename std::allocator_traits<Allocator>::const_pointer;
		using iterator				 = value_type *;
		using const_iterator		 = const value_type *;
		using reverse_iterator		 = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		constexpr DynArray() noexcept(noexcept(Allocator()));

		constexpr explicit DynArray(const Allocator & alloc) noexcept;

		constexpr DynArray(const DynArray & other, const Allocator & alloc = Allocator()) noexcept;

		constexpr DynArray(DynArray && other) noexcept;

		constexpr explicit DynArray(size_type count, const Allocator & alloc = Allocator()) noexcept;

		constexpr DynArray( std::initializer_list<T> initList, const Allocator & alloc = Allocator()) noexcept;

		constexpr explicit DynArray(const std::span<T> & span, const Allocator & alloc = Allocator()) noexcept;

		~DynArray() noexcept;

		constexpr DynArray & operator=(const DynArray & other) noexcept;
		constexpr DynArray & operator=(DynArray && other) noexcept;
		constexpr DynArray & operator=(const std::span<T> & span) noexcept;
		constexpr DynArray & operator=(std::initializer_list<T> initList) noexcept;

		constexpr Allocator get_allocator() const noexcept;

		constexpr T & at(size_type index);
		constexpr const T & at(size_type index) const;

		constexpr T & operator[](size_type index) noexcept;
		constexpr const T & operator[](size_type index) const noexcept;

		constexpr T & front() noexcept;
		constexpr const T & front() const noexcept;

		constexpr T & back() noexcept;
		constexpr const T & back() const noexcept;

		constexpr T * data() noexcept;
		constexpr const T * data() const noexcept;

		constexpr iterator begin() noexcept;
		constexpr const_iterator begin() const noexcept;
		constexpr const_iterator cbegin() const noexcept;

		constexpr iterator end() noexcept;
		constexpr const_iterator end() const noexcept;
		constexpr const_iterator cend() const noexcept;

		constexpr reverse_iterator rbegin() noexcept;
		constexpr const_reverse_iterator rbegin() const noexcept;
		constexpr const_reverse_iterator crbegin() const noexcept;

		constexpr reverse_iterator rend() noexcept;
		constexpr const_reverse_iterator rend() const noexcept;
		constexpr const_reverse_iterator crend() const noexcept;

		constexpr bool empty() const noexcept;

		constexpr size_type size() const noexcept;

		constexpr size_type max_size() const noexcept;

		constexpr void reserve(size_type newCapacity);

		constexpr size_type capacity() const noexcept;

		constexpr void shrink_to_fit();

		constexpr void clear() noexcept;

		constexpr iterator insert(const_iterator pos, const T & value);

		constexpr iterator insert(const_iterator pos, T && value);

		constexpr iterator insert(const_iterator pos, size_type count, const T & value);




		constexpr bool operator==(const DynArray & other) const noexcept;

		constexpr bool operator!=(const DynArray & other) const noexcept;

		constexpr void swap(DynArray & other) noexcept;











	private:




	};

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray() noexcept(noexcept(Allocator()))
	{
	}

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray(const Allocator & alloc) noexcept
	{
	}

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray(const DynArray & other, const Allocator & alloc) noexcept
	{
	}

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray(DynArray && other) noexcept
	{
	}

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray(DynArray::size_type count, const Allocator & alloc) noexcept
	{
	}

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray(std::initializer_list<T> initList, const Allocator & alloc) noexcept
	{
	}

	template <class T, std::size_t N, class Allocator>
	constexpr DynArray<T, N, Allocator>::DynArray(const std::span<T> & span, const Allocator & alloc) noexcept
	{
	}

	template <class T, std::size_t N, class Allocator>
	DynArray<T, N, Allocator>::~DynArray() noexcept
	{
	}

} // namespace gen