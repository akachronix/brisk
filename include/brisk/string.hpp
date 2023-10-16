#pragma once

#include <istream>
#include <ostream>
#include <iterator>

#include "utility.hpp"

namespace brisk
{
    // Forward declared so we can use them with the class
    brisk::size_t strlen(const char* s);    // DOES NOT include null character
    brisk::size_t strsize(const char* s);   // DOES include null character

    class string
    {        
    public:
        string()
        {
            m_size = 16;
            m_characters = 0;
            m_string = new char[m_size];
        }

        string(const char* s)
        {
            m_characters = brisk::strsize(s);
            m_size = m_characters << 2;
            m_string = new char[m_size];
            memcpy(m_string, s, m_characters);
            memset(&m_string[m_characters - 1], 0, m_size - m_characters);
        }

        string(const char c)
        {
            m_size = 16;
            m_characters = 2;
            m_string = new char[m_size];
            m_string[0] = c;
            memset(&m_string[1], 0 , m_size - m_characters);
        }

        string(size_t newSize)
        {
            m_size = newSize;
            m_characters = 0;
            m_string = new char[newSize];
            memset(m_string, 0, m_size);
        }

        string(const string& other)
        {
            m_size = other.m_size;
            m_characters = other.m_characters;
            m_string = new char[m_size];
            memcpy(m_string, other.m_string, m_size);
        }

        string(string&& other)
        {
            m_string = other.m_string;
            m_size = brisk::move(other.m_size);
            m_characters = brisk::move(other.m_characters);
            other.m_string = new char[16];
            other.m_size = 16;
            other.m_characters = 0;
        }

        ~string()
        {
            delete[] m_string;
        }

        string& operator=(const string& str)
        {
            m_characters = str.size();
            m_size = str.capacity();
            m_string = new char[m_size];
            memcpy(m_string, str.data(), m_size);
            return *this;
        }

        string& operator=(const char* s)
        {
            m_characters = brisk::strsize(s);
            if (m_size < m_characters) {
                realloc(m_characters << 2);
            }

            memcpy(m_string, s, m_characters);
            memset(&m_string[m_characters - 1], 0, m_size - m_characters);
            return *this;
        }
        
        string& operator=(char c)
        {
            printf("m_size: %u, m_characters: %u\n", m_size, m_characters);
            if (m_size < 2) {
                realloc(4);
            }

            m_string[0] = c;
            m_characters = 2; // +1 for null character
            memset(m_string + (m_characters - 1), 0, m_size - m_characters);
            return *this;
        }

        string& operator+=(const string& str)
        {
            this->append(str);
            return *this;
        }

        string& append(const string& str)
        {
            if (m_size < (m_characters + str.length())) {
                realloc((m_characters + str.length()) << 2);
            }

            memcpy(&m_string[m_characters - 1], str.data(), str.length());
            m_characters += str.length();
            memset(&m_string[m_characters - 1], 0, m_size - m_characters);

            return *this;
        }

        string& append(const char* s)
        {
            brisk::size_t sSize = strsize(s);
            if (m_size < (m_characters + (sSize - 1))) {
                realloc((m_characters + sSize) << 2);
            }

            memcpy(&m_string[m_characters - 1], s, sSize);
            m_characters += (sSize - 1);
            memset(&m_string[m_characters - 1], 0, m_size - m_characters);

            return *this;
        }

        string& append(char c)
        {
            if (m_size < (m_characters + 1)) {
                realloc(m_size << 2);
            }

            m_string[m_characters - 1] = c;
            m_characters++;
            memset(&m_string[m_characters - 1], 0, m_size - m_characters);

            return *this;
        }

        string& insert(size_t index, const string& s) 
        {
            // Need to fix
            return *this;
        }

        string& insert(size_t index, const char* s)
        {
            // Need to fix
            return *this;
        }

        string& insert(size_t index, char c)
        {
            // Need to fix
            return *this;
        }

        size_t size() const noexcept
        {
            return m_characters - 1;
        }

        size_t length() const noexcept
        {
            return m_characters - 1;
        }

        size_t capacity() const noexcept
        {
            return m_size;
        }

        void reserve(brisk::size_t newSize)
        {
            if (newSize > m_size) {
                realloc(newSize);
            }
        }

        char& operator[](size_t index)
        {
            return m_string[index];
        }

        const char& operator[](size_t index) const
        {
            return m_string[index];
        }

        char& at(size_t index)
        {
            if (index > m_characters) {
                throw std::out_of_range("[brisk::string][Exception]: Index out of range");
            }
            return m_string[index];
        }

        const char& at(size_t index) const
        {
            if (index > m_characters) {
                throw std::out_of_range("[brisk::string][Exception]: Index out of range");
            }
            return m_string[index];
        }

        char& front() noexcept
        {
            return m_string[0];
        }

        char& back() noexcept
        {
            return m_string[m_characters - 1];
        }

        const char& front() const noexcept
        {
            return m_string[0];
        }

        const char& back() const noexcept
        {
            return m_string[m_characters - 1];
        }

        const char* data() const noexcept
        {
            return m_string;
        }

        const char* c_str() const noexcept
        {
            return m_string;
        }

        char* begin() noexcept
        {
            return &m_string[0];
        }

        char* end() noexcept
        {
            return &m_string[m_characters - 1];
        }

        const char* cbegin() const noexcept
        {
            return &m_string[0];
        }

        const char* cend() const noexcept
        {
            return &m_string[m_characters - 1];
        }

        std::reverse_iterator<char*> rbegin() noexcept
        {
            return std::reverse_iterator<char*>(&m_string[m_characters - 1]);
        }

        std::reverse_iterator<char*> rend() noexcept
        {
            return std::reverse_iterator<char*>(&m_string[0]);
        }

        std::reverse_iterator<const char*> crbegin() const noexcept
        {
            return std::reverse_iterator<const char*>(&m_string[m_characters - 1]);
        }

        std::reverse_iterator<const char*> crend() const noexcept
        {
            return std::reverse_iterator<const char*>(&m_string[0]);
        }

        friend std::ostream& operator<<(std::ostream& out, const brisk::string& string);
	    friend std::istream& operator>>(std::istream& in, brisk::string& string);

    private:
        void realloc(const size_t newSize)
        {
            // Destination = (beginning of m_string) + (number of characters without null term)
            // Size = (allocated size) - (number of characters with null term)
            char* buffer = new char[newSize];
            if (newSize > m_size)
            {
                for (brisk::size_t i = 0; i < m_size; i++) {
                    buffer[i] = brisk::move(m_string[i]);
                }
            }

            else
            {
                for (brisk::size_t i = 0; i < newSize; i++) {
                    buffer[i] = brisk::move(m_string[i]);
                }
            }

            memset((buffer + strlen(buffer)), 0, (newSize - strsize(buffer)));

            delete[] m_string;
            m_string = buffer;
            m_size = newSize;
            m_characters = strsize(m_string);
        }

        char* m_string;
        size_t m_characters;    // Includes null-term char
        size_t m_size;          // Actual allocated size
    };
    
    std::ostream& operator<<(std::ostream& out, const brisk::string& string)
    {
        out << string.m_string;
        return out;
    }

    std::istream& operator>>(std::istream& in, brisk::string& string)
    {
        delete[] string.m_string;
        string.m_size = 256;
        string.m_string = new char[string.m_size];
        in.getline(string.m_string, 256, 10);
        string.m_characters = strsize(string.m_string);
        return in;
    }

    string operator+(const string& lhs, const string& rhs)
    {
        brisk::string result;
        result.append(lhs);
        result.append(rhs);
        return result;
    }

    string operator+(const string& lhs, const char* rhs)
    {
        brisk::string result;
        result.append(lhs);
        result.append(rhs);
        return result;
    }

    string operator+(const string& lhs, char rhs)
    {
        brisk::string result;
        result.append(lhs);
        result.append(rhs);
        return result;
    }

    string operator+(const char* lhs, const string& rhs)
    {
        brisk::string result;
        result.append(lhs);
        result.append(rhs);
        return result;
    }

    string operator+(char lhs, const string& rhs)
    {
        brisk::string result;
        result.append(lhs);
        result.append(rhs);
        return result;
    }

    string operator+(string&& lhs, string&& rhs)
    {
        lhs.append(rhs);
        return brisk::move(lhs);
    }

    string operator+(string&& lhs, const string& rhs)
    {
        lhs.append(rhs);
        return brisk::move(lhs);
    }

    string operator+(string&& lhs, const char* rhs)
    {
        lhs.append(rhs);
        return brisk::move(lhs);
    }

    string operator+(string&& lhs, char rhs)
    {
        lhs.append(rhs);
        return brisk::move(lhs);
    }

    string operator+(const string& lhs, string&& rhs)
    {
        // TODO: create insert() overload for string&
        rhs.insert(0, lhs);
        return brisk::move(rhs);
    }

    string operator+(const char* lhs, string&& rhs)
    {
        rhs.insert(0, lhs);
        return brisk::move(rhs);
    }

    string operator+(char lhs, string&& rhs)
    {
        rhs.insert(0, lhs);
        return brisk::move(rhs);
    }

    brisk::size_t strlen(const char* s)
    {
        const char* end = s;
        for (; *end != '\0'; ++end);
        return (end - s);
    }

    brisk::size_t strlen(const brisk::string& s)
    {
        return strlen(s.data());
    }

    brisk::size_t strsize(const char* s)
    {
        const char* end = s;
        for (; *end != '\0'; ++end);
        return (end - s) + 1;
    }

    brisk::size_t strsize(const brisk::string& s)
    {
        return strsize(s.data());
    }
}
