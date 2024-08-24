#pragma once

#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <cstring>

#include "briskdef.hpp"
#include "utility.hpp"

namespace brisk
{
	template <class Type>
	class vector
	{
	public:
		using size_type = brisk::size_t;
		using value_type = Type;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;
		using iterator = Type*;
		using const_iterator = const Type*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using difference_type = brisk::ptrdiff_t;

		vector()
			: m_elements(0), m_size(4), m_array(new Type[m_size])
		{
		}

		explicit vector(const size_type size)
			: m_elements(0), m_size(size), m_array(new Type[size])
		{
		}
		
		vector(const std::initializer_list<Type>&& list)
			: m_elements(list.size()), m_size(list.size() << 2), m_array(new Type[m_size])
		{
			for (typename std::initializer_list<Type>::iterator it = list.begin(); it != list.end(); ++it) {
				m_array[it - list.begin()] = *(it);
			}
		}
		
		vector(iterator const begin, iterator const end)
			: m_elements(0), m_size(0)
		{
			for (iterator it = begin; it != end; ++it) {
				m_elements++;
			}
			
			m_size = m_elements << 2;
			m_array = new Type[m_size];
			for (size_type i = 0; i < m_elements; ++i) {
				m_array[i] = *(begin + i);
			}
		}

		vector(const vector& v2)
			: m_elements(v2.m_elements), m_size(v2.m_size), m_array(new Type[m_size])
		{
			for (size_type i = 0; i < v2.m_size; i++) {
				m_array[i] = v2.m_array[i];
			}
		}

		vector(vector&& v2)
			: m_elements(brisk::move(v2.m_elements)), m_size(brisk::move(v2.m_size)), m_array(v2.m_array)
		{		
			v2.m_elements = 0;
			v2.m_size = 0;
			v2.m_array = nullptr;
		}

		virtual ~vector()
		{
			delete[] m_array;
		}

		vector<Type>& operator=(const vector& v2)
		{
			m_elements = v2.m_elements;
			m_size = v2.m_size;
			m_array = new Type[m_size];
			for (size_type i = 0; i < v2.m_size; i++) {
				m_array[i] = v2.m_array[i];
			}

			return *this;
		}

		vector<Type>& operator=(vector&& v2) noexcept
		{
			m_elements = brisk::move(v2.m_elements);
			m_size = brisk::move(v2.m_size);
			m_array = v2.m_array;
			v2.m_elements = 0;
			v2.m_size = 0;
			v2.m_array = nullptr;

			return *this;
		}

		reference operator[](const size_type index) noexcept
		{
			return m_array[index];
		}

		const_reference operator[](const size_type index) const noexcept
		{
			return m_array[index];
		}

		reference at(const size_type index)
		{
			if (index >= m_elements) {
				throw std::out_of_range("[brisk::vector][Exception]: Index out of range");
			}
			
			return m_array[index];
		}

		const_reference at(const size_type index) const
		{
			if (index >= m_elements) {
				throw std::out_of_range("[brisk::vector][Exception]: Index out of range");
			}
			
			return m_array[index];
		}

		reference front() noexcept
		{
			return m_array[0];
		}

		const_reference front() const noexcept
		{
			return m_array[0];
		}

		reference back() noexcept
		{
			return m_array[m_elements - 1];
		}

		const_reference back() const noexcept
		{
			return m_array[m_elements - 1];
		}

		pointer data() noexcept
		{
			return m_array;
		}

		const_pointer data() const noexcept
		{
			return m_array;
		}

		template <class... Args>
		void emplace_back(Args&&... args)
		{
			if (m_elements == m_size) {
				realloc(m_size << 2);
			}

			m_array[m_elements] = brisk::move(Type(brisk::forward<Args>(args)...));
			++m_elements;
		}
		
		void push_back(const std::initializer_list<Type>&& list)
		{
			for (typename std::initializer_list<Type>::const_iterator it = list.begin(); it != list.end(); ++it)
			{
				if (m_elements == m_size) {
					realloc(m_size << 2);
				}

				m_array[m_elements] = *(it);
				++m_elements;
			}
		}

		void push_back(const Type& value)
		{
			if (m_elements == m_size) {
				realloc(m_size << 2);
			}

			m_array[m_elements] = value;
			++m_elements;
		}

		void push_back(const Type&& value)
		{
			if (m_elements == m_size) {
				realloc(m_size << 2);
			}

			m_array[m_elements] = brisk::move(value);
			++m_elements;
		}

		void pop_back()
		{
			if (m_elements != 0) {
				m_array[m_elements].~Type();
				--m_elements;
			}
		}

		size_type capacity() const noexcept
		{
			return m_size;
		}

		size_type size() const noexcept
		{
			return m_elements;
		}

		bool empty() const noexcept
		{
			return (m_elements == 0) ? true : false;
		}

		explicit operator bool() const noexcept
		{
			return (m_elements == 0) ? false : true;
		}

		void resize(const size_type size)
		{
			realloc(size);
		}

		void reserve(const size_type size)
		{
			if (size > m_size) {
				realloc(size);
			}
		}

		void shrink_to_fit()
		{
			if ((m_elements * 2) < m_size) {
				realloc(m_elements);
			}
		}

		void fill(const value_type& value) noexcept
		{
			for (size_type i = 0; i < m_elements; ++i) {
				m_array[i] = value;
			}
		}

		bool operator==(const vector<Type>& rhs) const noexcept
		{
			if (m_elements != rhs.m_elements) {
				return false;
			}
			
			for (size_type i = 0; i < m_elements; ++i)
			{
				if (m_array[i] != rhs.m_array[i]) {
					return false;
				}
			}
			
			return true;
		}

		bool operator!=(const vector<Type>& rhs) const noexcept
		{
			if (m_elements != rhs.m_elements) {
				return true;
			}
			
			for (size_type i = 0; i < m_elements; ++i)
			{
				if (m_array[i] != rhs.m_array[i]) {
					return true;
				}
			}
			
			return false;
		}
        
        void clear() noexcept
        {
            for (int i = 0; i < m_elements; ++i) {
                m_array[i].~Type();
			}
            
            m_elements = 0;
            m_size = 0;
        }

		iterator erase(const_iterator position)
		{
			iterator it = &m_array[position - m_array];
			(*it).~Type();
            memmove(it, it + 1, (m_size - (it - m_array) - 1) * sizeof(Type));
			--m_elements;
			return it;
		}

		iterator erase(const_iterator first, const_iterator last)
		{	
			iterator f = &m_array[first - m_array];
			if (first == last) {
				return f;
			}

			for (; first != last; ++first) {
				(*first).~Type();
			}

			memmove(f, last, (m_size - (last - m_array)) * sizeof(Type));
			m_elements -= last - first;
			return f;
		}

		iterator begin() noexcept
		{
			return &m_array[0];
		}

		iterator end() noexcept
		{
			return &m_array[m_elements];
		}
		
		const_iterator cbegin() const noexcept
		{
			return &m_array[0];
		}
		
		const_iterator cend() const noexcept
		{
			return &m_array[m_elements];
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(&m_array[m_elements]);
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(&m_array[0]);
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return reverse_iterator(&m_array[m_elements]);
		}

		const_reverse_iterator crend() const noexcept
		{
			return reverse_iterator(&m_array[0]);
		}

	private:
		void realloc(const size_t newSize)
		{
			// If newSize is > current size, we move every object to new buffer
			if (newSize > m_size) 
			{
				Type* buffer = new Type[newSize];
				for (size_type i = 0; i < m_size; ++i) {
					buffer[i] = brisk::move(m_array[i]);
				}
				
				delete[] m_array;
				m_array = buffer;
				m_size = newSize;
			} 
			
			// Array never "shrinks" however this cuts down on unnecessary
			// allocations and makes us way way way faster
			else 
			{
				erase(begin() + newSize, end());
				m_elements = newSize;
			}
		}

	private:
		size_type m_elements;
		size_type m_size;
		value_type* m_array;
	};
}
