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
		typedef brisk::size_t size_type;
		typedef Type value_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef Type* iterator;
		typedef const Type* const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef brisk::ptrdiff_t difference_type;

		vector()
		{
			m_elements = 0;
			m_size = 4;
			m_array = new Type[m_size];
		}

		explicit vector(const size_type size)
		{
			m_elements = 0;
			m_size = size << 2;
			m_array = new Type[m_size];
		}
		
		vector(const std::initializer_list<Type> list)
		{
			m_elements = list.size();
			m_size = list.size() << 2;
			m_array = new Type[m_size];

			for (auto it = list.begin(); it != list.end(); ++it) {
				m_array[it - list.begin()] = *(it);
			}
		}
		
		vector(iterator const begin, iterator const end)
		{
			m_elements = 0;
			m_size = 0;
            
			for (iterator it = begin; it != end; ++it) {
				++m_elements;
			}
			
			m_size = m_elements << 2;
			m_array = new Type[m_size];
			for (size_type i = 0; i < m_elements; ++i) {
				m_array[i] = *(begin + i);
			}
		}

		vector(const vector& v2)
		{
			m_elements = v2.m_elements;
			m_size = v2.m_size;
			m_array = new Type[m_size];
			memcpy(m_array, v2.m_array, v2.m_elements * sizeof(Type));
		}

		vector(vector&& v2)
		{
			m_elements = brisk::move(v2.m_elements);
			m_size = brisk::move(v2.m_size);
			m_array = v2.m_array;			
			v2.m_elements = 0;
			v2.m_size = 0;
			v2.m_array = nullptr;
		}

		~vector()
		{
			delete[] m_array;
		}

		vector<Type>& operator=(const vector& v2)
		{
			m_elements = v2.m_elements;
			m_size = v2.m_size;
			m_array = new Type[m_size];
			memcpy(m_array, v2.m_array, v2.m_elements * sizeof(Type));		
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
			if (m_elements == m_size)
			{
				size_type old_m_size = m_size;
				m_size <<= 2;

				Type* buffer = new Type[m_size];
				for (size_type i = 0; i < old_m_size; ++i) {
					buffer[i] = brisk::move(m_array[i]);
				}

				delete[] m_array;
				m_array = buffer;
			}

			m_array[m_elements] = brisk::move(Type(brisk::forward<Args>(args)...));
			++m_elements;
		}
		
		void push_back(const std::initializer_list<Type>&& list)
		{
			for (auto it = list.begin(); it != list.end(); ++it)
			{
				if (m_elements == m_size)
				{
					m_size <<= 2;
					Type* buffer = new Type[m_size];
					memcpy(buffer, m_array, m_elements * sizeof(Type));
					delete[] m_array;
					m_array = buffer;
				}

				m_array[m_elements] = *(it);
				++m_elements;
			}
		}

		void push_back(const Type& value)
		{
			if (m_elements == m_size)
			{
				size_type old_m_size = m_size;
				m_size <<= 2;

				Type* buffer = new Type[m_size];
				for (size_type i = 0; i < old_m_size; ++i) {
					buffer[i] = brisk::move(m_array[i]);
				}

				delete[] m_array;
				m_array = buffer;
			}

			m_array[m_elements] = value;
			++m_elements;
		}

		void push_back(const Type&& value)
		{
			if (m_elements == m_size)
			{
				size_type old_m_size = m_size;
				m_size <<= 2;

				Type* buffer = new Type[m_size];
				for (size_type i = 0; i < old_m_size; ++i) {
					buffer[i] = brisk::move(m_array[i]);
				}

				delete[] m_array;
				m_array = buffer;
			}

			m_array[m_elements] = brisk::move(value);
			++m_elements;
		}

		void pop_back()
		{
			--m_elements;
			m_array[m_elements].~Type();
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
			if (size > m_size)
			{
				size_type old_m_size = m_size;
				m_size = size;

				Type* buffer = new Type[m_size];
				for (size_type i = 0; i < old_m_size; ++i) {
					buffer[i] = brisk::move(m_array[i]);
				}

				delete[] m_array;
				m_array = buffer;
			}

			m_elements = size;
		}

		void reserve(const size_type size)
		{
			if (size > m_size)
			{
				size_type old_m_size = m_size;
				m_size = size;
				
				Type* buffer = new Type[m_size];
				for (size_type i = 0; i < old_m_size; ++i) {
					buffer[i] = brisk::move(m_array[i]);
				}

				delete[] m_array;
				m_array = buffer;
			}
		}

		void shrink_to_fit()
		{
			m_size = m_elements;
			Type* buffer = new Type[m_size];
			for (size_type i = 0; i < m_size; ++i) {
				buffer[i] = brisk::move(m_array[i]);
			}

			delete[] m_array;
			m_array = buffer;
		}

		void fill(const value_type& value)
		{
			for (size_type i = 0; i < m_elements; ++i) {
				m_array[i] = value;
			}
		}

		bool operator==(const vector<Type>& rhs) const
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

		bool operator!=(const vector<Type>& rhs) const
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
		size_type m_elements;
		size_type m_size;
		value_type* m_array;
	};
}
