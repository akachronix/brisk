#pragma once

#include "vector.hpp"
#include "string.hpp"
#include "utility.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

namespace brisk
{
	class logger
	{
	public:
		logger(brisk::string logfile)
		{
			m_logFile = logfile;
			m_amILogging = true;
			m_amIPrinting = true;
		}

		logger(const logger& other)
		{
			logHistory = other.logHistory;
			m_logFile = other.m_logFile;
			m_amILogging = other.m_amILogging;
			m_amIPrinting = other.m_amIPrinting;
		}

		logger(logger&& other) noexcept
		{
			logHistory = brisk::move(other.logHistory);
			m_logFile = brisk::move(other.m_logFile);
			m_amILogging = brisk::move(other.m_amILogging);
			m_amIPrinting = brisk::move(other.m_amIPrinting);

			other.m_amILogging = true;
			other.m_amIPrinting = true;
		}

		~logger()
		{
			dumpLog(m_logFile);
		}

		logger& operator=(const logger& other)
		{
			logHistory = other.logHistory;
			m_logFile = other.m_logFile;
			m_amILogging = other.m_amILogging;
			m_amIPrinting = other.m_amIPrinting;

			return *this;
		}
		
		logger& operator=(logger&& other) noexcept
		{
			if (this != &other)
			{
				logHistory = brisk::move(other.logHistory);
				m_logFile = brisk::move(other.m_logFile);
				m_amILogging = brisk::move(other.m_amILogging);
				m_amIPrinting = brisk::move(other.m_amIPrinting);

				other.m_amILogging = true;
				other.m_amIPrinting = true;
			}

			return *this;
		}

		const brisk::string& filename() const noexcept
		{
			return m_logFile;
		}

		void filename(const char* filename) noexcept
		{
			m_logFile = filename;
		}


		template <class T>
		void print(T value)
		{
			std::stringstream casted_value;
			casted_value << value;
			if (m_amIPrinting) {
				std::cout << value;
			}
			logHistory.push_back(casted_value.str().c_str());
		}

		void print(logger&(*func)(logger&))
		{
			func(*this);
		}

		template<class T>
		void input(T& var)
		{
			std::cin >> var;
			std::stringstream varToString;
			varToString << var << "\n";
			logHistory.push_back(varToString.str().c_str());
		}

		const brisk::vector<brisk::string>& buffer() const noexcept
		{
			return logHistory;
		}

		size_t size() noexcept
		{
			size_t sz = 0;
			for (brisk::string& it : logHistory) {
				sz += it.size();
			}

			return sz;
		}

		void shrink_to_fit()
		{
			brisk::string s;
			for (brisk::string& it : logHistory) {
				s.append(it);
			}
			
			logHistory.clear();
			logHistory.emplace_back(s);
			logHistory.shrink_to_fit();
		}

		bool dumpLog(const brisk::string file)
		{
			if (logHistory.size() != 0 && m_amILogging == true)
			{
				std::ofstream log_file(file.c_str());
				if (log_file.is_open())
				{
					for (brisk::string& x : logHistory) {
						log_file << x;
					}

					log_file.close();
					return true;
				}
			}
			
			return false;
		}

		bool dumpLog()
		{
			return dumpLog(m_logFile);
		}

		void disableLogging() noexcept
		{
			m_amILogging = false;
		}

		void enableLogging() noexcept
		{
			m_amILogging = true;
		}

		void disablePrinting() noexcept
		{
			m_amIPrinting = false;
		}

		void enablePrinting() noexcept
		{
			m_amIPrinting = true;
		}

	private:
		brisk::vector<brisk::string> logHistory;
		brisk::string m_logFile;
		bool m_amILogging;
		bool m_amIPrinting;
	};

	template <class T>
	inline logger& operator<<(logger& log, T value)
	{
		log.print(value);
		return log;
	}

	inline logger& operator<<(logger& log, logger&(*func)(logger&))
	{
		return (*func)(log);
	}

	template <class T>
	inline logger& operator>>(logger& log, T& value)
	{
		log.input(value);
		return log;
	}

	inline logger& newl(logger& log)
	{
		log.print("\n");
		return log;
	}

	inline logger& flush(logger& log)
	{
		std::cout.flush();
		return log;
	}

	inline logger& stab(logger& log)
	{
		log.print("  ");
		return log;
	}

	inline logger& tab(logger& log)
	{
		log.print("    ");
		return log;
	}

	inline logger& space(logger& log)
	{
		log.print(" ");
		return log;
	}
}
