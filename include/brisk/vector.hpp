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
        // Type Definitions
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

        // Constructors / Destructor
        vector();
        explicit vector(const size_type size);
        vector(const std::initializer_list<Type>&& list);
        vector(iterator const begin, iterator const end);
        vector(const vector& v2);   // Copy constructor
        vector(vector&& v2);        // Move constructor
        virtual ~vector();
        
        // Equals operators
        vector<Type>& operator=(const vector<Type>& v2);
        vector<Type>& operator=(vector&& v2) noexcept;
        bool operator==(const vector<Type>& rhs) const noexcept;
        bool operator!=(const vector<Type>& rhs) const noexcept;

        // Array operators
        reference operator[](const size_type index) noexcept;
        const_reference operator[](const size_type index) const noexcept;

        // Value modifying methods
        template <class... Args> iterator emplace(iterator pos, Args&&... args);
        template <class... Args> void emplace_back(Args&&... args);
        void push_back(const std::initializer_list<Type>&& list);
        void push_back(const Type& value);
        void push_back(const Type&& value);
        void pop_back();
        void assign(size_type count, const Type& value);

        // Size methods / erasure
        size_type capacity() const noexcept;
        size_type size() const noexcept;
        bool empty() const noexcept;
        explicit operator bool() const noexcept;
        void resize(const size_type size);
        void reserve(const size_type size);
        void shrink_to_fit();
        void fill(const value_type& value) noexcept;
        void clear() noexcept;
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        // Location helper functions
        reference at(const size_type index);
        const_reference at(const size_type index) const;
        reference front() noexcept;
        const_reference front() const noexcept;
        reference back() noexcept;
        const_reference back() const noexcept;
        pointer data() noexcept;
        const_pointer data() const noexcept;
        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;
        
    private:
        void realloc(const size_t newSize);

    private:
        size_type m_elements;
        size_type m_size;
        value_type* m_array;
    };

    // All reallocation logic lies here.
    // 
    // Value modifying functions check bounds and call for reallocation if
    // needed. When realloc is called it's either directly given a size, 
    // or is (n << 2) [faster way of multiplying by 4]
    template <class Type>
    void vector<Type>::realloc(const size_t newSize)
    {
        bool reallocate = false;
        size_t reallocSz = 0;

        if (newSize * 4 < m_size) {
            reallocSz = m_elements;
            reallocate = true;
        }

        if (newSize >= m_size) {
            reallocSz = newSize;
            reallocate = true;
        }

        reallocSz = (reallocSz == 0) ? 4 : reallocSz;

        if (reallocate == true) 
        {
            Type* buffer = new Type[reallocSz];
            for (size_type i = 0; i < m_elements; ++i) {
                buffer[i] = brisk::move(m_array[i]);
            }
            
            delete[] m_array;
            m_array = buffer;
            m_size = reallocSz;
        }
    }

    // Constructors / Destructor
    // ------------------------------------------------------
    // vector();
    // explicit vector(const size_type size);
    // vector(const std::initializer_list<Type>&& list);
    // vector(iterator const begin, iterator const end);
    // vector(const vector& v2);   // Copy constructor
    // vector(vector&& v2);        // Move constructor
    // virtual ~vector();
    // ------------------------------------------------------
    template <class Type>
    vector<Type>::vector()
        :   m_elements(0), 
            m_size(4), 
            m_array(new Type[4])
    {}
    
    template <class Type>
    vector<Type>::vector(const size_type size)
        :   m_elements(0), 
            m_size(size), 
            m_array(new Type[size])
    {}
    
    template <class Type>
    vector<Type>::vector(const std::initializer_list<Type>&& list)
        :   m_elements(list.size()), 
            m_size(list.size() << 2), 
            m_array(new Type[list.size() << 2])
    {
        for (typename std::initializer_list<Type>::iterator it = list.begin(); it != list.end(); ++it) {
            m_array[it - list.begin()] = *(it);
        }
    }
    
    template <class Type>
    vector<Type>::vector(iterator const begin, iterator const end)
        :   m_elements(0), 
            m_size(0)
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

    template <class Type>
    vector<Type>::vector(const vector<Type>& v2)
        : m_elements(v2.m_elements), m_size(v2.m_size), m_array(new Type[m_size])
    {
        for (size_type i = 0; i < v2.m_size; i++) {
            m_array[i] = v2.m_array[i];
        }
    }

    template <class Type>
    vector<Type>::vector(vector<Type>&& v2)
        : m_elements(brisk::move(v2.m_elements)), m_size(brisk::move(v2.m_size)), m_array(v2.m_array)
    {       
        v2.m_elements = 0;
        v2.m_size = 0;
        v2.m_array = nullptr;
    }

    template <class Type>
    vector<Type>::~vector() {
        delete[] m_array;
    }

    // Equals operators
    // ---------------------------------------------------------
    // vector<Type>& operator=(const vector<Type>& v2);
    // vector<Type>& operator=(vector&& v2) noexcept;
    // bool operator==(const vector<Type>& rhs) const noexcept;
    // bool operator!=(const vector<Type>& rhs) const noexcept;
    // ---------------------------------------------------------
    template <class Type>
    vector<Type>& vector<Type>::operator=(const vector<Type>& v2)
    {
        m_elements = v2.m_elements;
        m_size = v2.m_size;
        m_array = new Type[m_size];
        
        for (size_type i = 0; i < v2.m_size; i++) {
            m_array[i] = v2.m_array[i];
        }

        return *this;
    }

    template <class Type>
    vector<Type>& vector<Type>::operator=(vector<Type>&& v2) noexcept
    {
        m_elements = brisk::move(v2.m_elements);
        m_size = brisk::move(v2.m_size);
        m_array = v2.m_array;
        v2.m_elements = 0;
        v2.m_size = 0;
        v2.m_array = nullptr;

        return *this;
    }

    template <class Type>
    bool vector<Type>::operator==(const vector<Type>& rhs) const noexcept
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

    template <class Type>
    bool vector<Type>::operator!=(const vector<Type>& rhs) const noexcept
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

    // Array operators
    // ------------------------------------------------------------------------
    // reference operator[](const size_type index) noexcept;
    // const_reference operator[](const size_type index) const noexcept;
    // ------------------------------------------------------------------------
    template <class Type>
    vector<Type>::reference vector<Type>::operator[](const vector<Type>::size_type index) noexcept {
        return m_array[index];
    }

    template <class Type>
    vector<Type>::const_reference vector<Type>::operator[](const vector<Type>::size_type index) const noexcept {
        return m_array[index];
    }

    // Value modifying methods
    // ------------------------------------------------------------------------
    // template <class... Args> iterator emplace(iterator pos, Args&&... args);
    // template <class... Args> void emplace_back(Args&&... args);
    // void push_back(const std::initializer_list<Type>&& list);
    // void push_back(const Type& value);
    // void push_back(const Type&& value);
    // void pop_back();
    // void assign(size_type count, const Type& value);
    // ------------------------------------------------------------------------
    template <class Type>
    template <class... Args>
    vector<Type>::iterator vector<Type>::emplace(typename vector<Type>::iterator pos, Args&&... args)
    {
        if (pos < this->begin() || pos > this->end()) {
            throw std::overflow_error("[brisk::vector][Exception]: Iterator out of range");
        }
        
        if (m_size <= m_elements+1) {
            realloc(m_size << 2);
        }
        
        // Start from the end, move backwards while moving each element over 1 until
        // we're at the end. If statement catches when the pos is == to v.begin(),
        // otherwise memmove would corrupt memory by going out of bounds
        iterator it = this->end();
        for (; it > pos; it--) {
            if (it - 1 < m_array) {
                break;
            }
            // it initializes to one past the last element, so start there in that empty space (it)
            // and move the previous element (it - 1) over sizeof(*it) bytes or the size of one element
            memmove(it, it-1, sizeof(*it));
            //     dest, src, bytes to copy
        }

        *it = brisk::move(brisk::forward<Args>(args)...);
        m_elements++;
        return it;
    }
    
    template <class Type>
    template <class... Args>
    void vector<Type>::emplace_back(Args&&... args)
    {
        if (m_size <= m_elements) {
            realloc(m_size << 2);
        }

        m_array[m_elements] = brisk::move(Type(brisk::forward<Args>(args)...));
        ++m_elements;
    }
    
    template <class Type>
    void vector<Type>::push_back(const std::initializer_list<Type>&& list)
    {
        for (typename std::initializer_list<Type>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (m_size <= m_elements) {
                realloc(m_size << 2);
            }

            m_array[m_elements] = *(it);
            ++m_elements;
        }
    }

    template <class Type>
    void vector<Type>::push_back(const Type& value)
    {
        if (m_size <= m_elements) {
            realloc(m_size << 2);
        }

        m_array[m_elements] = value;
        ++m_elements;
    }

    template <class Type>
    void vector<Type>::push_back(const Type&& value)
    {
        if (m_size <= m_elements) {
            realloc(m_size << 2);
        }

        m_array[m_elements] = brisk::move(value);
        ++m_elements;
    }

    template <class Type>
    void vector<Type>::pop_back() 
    {
        if (m_elements != 0) {
            m_array[m_elements].~Type();
            --m_elements;
        }
    }

    template <class Type>
    void vector<Type>::assign(size_type count, const Type& value) 
    {
        if (m_size <= count) {
            realloc(count << 2);
        }

        for (size_type i = 0; i < count; ++i) {
            m_array[i] = value;
        }

        m_elements += count - m_elements;
    }

    // Size methods / erasure
    // ----------------------------------------------
    // size_type capacity() const noexcept;
    // size_type size() const noexcept;
    // bool empty() const noexcept;
    // explicit operator bool() const noexcept;
    // void resize(const size_type size);
    // void reserve(const size_type size);
    // void shrink_to_fit();
    // void fill(const value_type& value) noexcept;
    // void clear() noexcept;
    // iterator erase(const_iterator pos);
    // iterator erase(const_iterator first, const_iterator last);
    // ----------------------------------------------
    template <class Type>
    vector<Type>::size_type vector<Type>::capacity() const noexcept {
        return m_size;
    }

    template <class Type>
    vector<Type>::size_type vector<Type>::size() const noexcept {
        return m_elements;
    }

    template <class Type>
    bool vector<Type>::empty() const noexcept {
        return (m_elements == 0) ? true : false;
    }

    template <class Type>
    vector<Type>::operator bool() const noexcept {
        return (m_elements == 0) ? false : true;
    }

    template <class Type>
    void vector<Type>::resize(const size_type size) {
        realloc(size);
    }

    template <class Type>
    void vector<Type>::reserve(const size_type size) {
        if (size > m_size) {
            realloc(size);
        }
    }

    template <class Type>
    void vector<Type>::shrink_to_fit() {
        if ((m_elements * 2) < m_size) {
            realloc(m_elements);
        }
    }

    template <class Type>
    void vector<Type>::fill(const value_type& value) noexcept {
        for (size_type i = 0; i < m_elements; ++i) {
            m_array[i] = value;
        }
    }
    
    template <class Type>
    void vector<Type>::clear() noexcept
    {
        for (int i = 0; i < m_elements; ++i) {
            m_array[i].~Type();
        }
        
        m_elements = 0;
        m_size = 0;
        erase(begin(), end());
    }

    template <class Type>
    vector<Type>::iterator vector<Type>::erase(const_iterator pos)
    {
        iterator iit = &m_array[pos - m_array];
        (*iit).~Type();
        memmove(iit, iit + 1, (m_elements - (pos - m_array) - 1) * sizeof(Type));
        --m_elements;
        return iit;
    }

    template <class Type>
    vector<Type>::iterator vector<Type>::erase(const_iterator first, const_iterator last) {
        iterator it = const_cast<iterator>(last-1);
        for (; it >= first; --it) {
            erase(it);
        }

        return it;
    }

    // Location helper functions
    // ---------------------------------------------------
    // reference at(const size_type index);
    // const_reference at(const size_type index) const;
    // reference front() noexcept;
    // const_reference front() const noexcept;
    // reference back() noexcept;
    // const_reference back() const noexcept;
    // pointer data() noexcept;
    // const_pointer data() const noexcept;
    // iterator begin() noexcept;
    // iterator end() noexcept;
    // const_iterator cbegin() const noexcept;
    // const_iterator cend() const noexcept;
    // reverse_iterator rbegin() noexcept;
    // reverse_iterator rend() noexcept;
    // const_reverse_iterator crbegin() const noexcept;
    // const_reverse_iterator crend() const noexcept;
    // ---------------------------------------------------
    template <class Type>
    vector<Type>::reference vector<Type>::at(const size_type index)
    {
        if (index >= m_elements) {
            throw std::out_of_range("[brisk::vector][Exception]: Index out of range");
        }
        
        return m_array[index];
    }

    template <class Type>
    vector<Type>::const_reference vector<Type>::at(const size_type index) const
    {
        if (index >= m_elements) {
            throw std::out_of_range("[brisk::vector][Exception]: Index out of range");
        }
        
        return m_array[index];
    } 

    template <class Type>
    vector<Type>::reference vector<Type>::front() noexcept {
        return m_array[0];
    }

    template <class Type>
    vector<Type>::const_reference vector<Type>::front() const noexcept {
        return m_array[0];
    }

    template <class Type>
    vector<Type>::reference vector<Type>::back() noexcept {
        return m_array[m_elements - 1];
    }

    template <class Type>
    vector<Type>::const_reference vector<Type>::back() const noexcept {
        return m_array[m_elements - 1];
    }

    template <class Type>
    vector<Type>::pointer vector<Type>::data() noexcept {
        return m_array;
    }

    template <class Type>
    vector<Type>::const_pointer vector<Type>::data() const noexcept {
        return m_array;
    }

    template <class Type>
    vector<Type>::iterator vector<Type>::begin() noexcept {
        return &m_array[0];
    }

    template <class Type>
    vector<Type>::iterator vector<Type>::end() noexcept {
        return &m_array[m_elements];
    }
    
    template <class Type>
    vector<Type>::const_iterator vector<Type>::cbegin() const noexcept {
        return &m_array[0];
    }
    
    template <class Type>
    vector<Type>::const_iterator vector<Type>::cend() const noexcept {
        return &m_array[m_elements];
    }

    template <class Type>
    vector<Type>::reverse_iterator vector<Type>::rbegin() noexcept {
        return reverse_iterator(&m_array[m_elements]);
    }

    template <class Type>
    vector<Type>::reverse_iterator vector<Type>::rend() noexcept {
        return reverse_iterator(&m_array[0]);
    }

    template <class Type>
    vector<Type>::const_reverse_iterator vector<Type>::crbegin() const noexcept {
        return reverse_iterator(&m_array[m_elements]);
    }

    template <class Type>
    vector<Type>::const_reverse_iterator vector<Type>::crend() const noexcept {
        return reverse_iterator(&m_array[0]);
    }
}
