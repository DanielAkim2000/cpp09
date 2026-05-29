#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>
#include <exception>
#include <cfloat>
#include <cctype>
#include <cstdlib>
#include <cfloat>
#include <cerrno>
#include <fstream>
#include <sstream>

class Date
{
private:
	Date();

public:
	static int getMaxDayforMonth(int month, int year);
	static bool isLeapYear(int year);
};

class BitcoinExchange
{
private:
	std::map<std::string, float> _databases;

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	class Parsing
	{
	private:
		Parsing();
		class InputFileIsInvalid : public std::exception
		{
		public:
			const char *what() const throw();
		};
		class DatabaseFileIsInvalid : public std::exception
		{
		public:
			const char *what() const throw();
		};

	public:
		static bool isValidDate(const std::string &date);
		static bool isValidNumber(const std::string &nb);
		static bool isValidRate(const std::string &rate);
		static bool isValidLineDatabaseFile(const std::string &rate);
		static bool isValidDatabaseFile(const std::string &nameFile);
		static bool isValidLineInputFile(const std::string &rate);
		// a faire prochainement
		static std::string getDateInDatabase(const std::string &line);
		static float getRateInDatabase(const std::string &line);
		static std::string getDateInInputFile(const std::string &line);
		static float getNbInInputFile(const std::string &line);
		static void parseInput(const std::string &file);
		static std::map<std::string, float> parseDatabase(const std::string &file);
	};

	void convertData(std::string databaseFile, std::string inputFile);
	void convertAllInputLine(std::string inputFile);
	float searchValtoLowerDate(std::string date);
};

#endif
