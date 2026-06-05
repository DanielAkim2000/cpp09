#include "BitcoinExchange.hpp"

const char *BitcoinExchange::Parsing::InputFileIsInvalid::what() const throw()
{
	return "Input file not valid";
}

const char *BitcoinExchange::Parsing::DatabaseFileIsInvalid::what() const throw()
{
	return "Database file not valid";
}

bool Date::isLeapYear(int year)
{
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int Date::getMaxDayforMonth(int month, int year)
{
	switch (month)
	{
	case 1:
		return (31);
		break;
	case 2:
		if (Date::isLeapYear(year))
			return (29);
		return (28);
		break;
	case 3:
		return (31);
		break;
	case 4:
		return (30);
		break;
	case 5:
		return (31);
		break;
	case 6:
		return (30);
		break;
	case 7:
		return (31);
		break;
	case 8:
		return (31);
		break;
	case 9:
		return (30);
		break;
	case 10:
		return (31);
		break;
	case 11:
		return (30);
		break;
	case 12:
		return (31);
		break;
	default:
		throw std::runtime_error("Mois non reconnue");
		break;
	}
}

BitcoinExchange::BitcoinExchange() : _databases() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		_databases = other._databases;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

bool BitcoinExchange::Parsing::isValidDate(const std::string &date)
{
	std::string year;
	std::string month;
	std::string day;
	std::string::iterator it;

	year = date.substr(0, 4);
	if (date.length() != 10)
		return (false);
	it = year.begin();
	while (it != year.end())
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)))
			return (false);
		it++;
	}
	if (date.at(4) != '-')
		return (false);
	month = date.substr(5, 2);
	it = month.begin();
	while (it != month.end())
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)))
			return (false);
		it++;
	}
	if (std::atoi(month.c_str()) > 12 || std::atoi(month.c_str()) < 1)
		return (false);
	if (date.at(7) != '-')
		return (false);
	day = date.substr(8, 2);
	it = day.begin();
	while (it != day.end())
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)))
			return (false);
		it++;
	}
	if (std::atoi(day.c_str()) < 1 || std::atoi(day.c_str()) > Date::getMaxDayforMonth(std::atoi(month.c_str()), std::atoi(year.c_str())))
		return (false);
	return (true);
}

bool BitcoinExchange::Parsing::isValidNumber(const std::string &nb)
{
	std::string::const_iterator it;
	int nbComma = 0;
	size_t i = 0;

	if (nb.empty())
		throw std::runtime_error("Error: bad input.");
	if (nb[0] == '+' || nb[0] == '-')
		i = 1;
	if (i == nb.length())
		throw std::runtime_error("Error: bad input.");
	for (it = nb.begin() + i; it != nb.end(); ++it)
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)) && *it != '.')
			throw std::runtime_error("Error: bad input.");
		if (*it == '.')
		{
			if (nbComma == 1)
				throw std::runtime_error("Error: bad input.");
			nbComma++;
		}
	}
	std::string::size_type pos = nb.find('.');
	if (pos != std::string::npos && pos + 1 == nb.length())
		throw std::runtime_error("Error: bad input.");
	char *end = NULL;
	errno = 0;
	double v = std::strtod(nb.c_str(), &end);
	if (end == nb.c_str() || *end != '\0')
		throw std::runtime_error("Error: bad input.");
	if (errno == ERANGE)
		throw std::runtime_error("Error: too large a number.");
	if (v > FLT_MAX)
		throw std::runtime_error("Error: too large a number.");
	if (v < 0.0)
		throw std::runtime_error("Error: not a positive number.");
	if (v > 1000.0)
		throw std::runtime_error("Error: too large a number.");
	return (true);
}

bool BitcoinExchange::Parsing::isValidRate(const std::string &rate)
{
	std::string::const_iterator it;
	int nbComma = 0;

	it = rate.begin();
	while (it != rate.end())
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)) && *it != '.')
			return false;
		if (*it == '.')
		{
			if (nbComma == 1)
				return false;
			nbComma++;
		}
		it++;
	}
	std::string::size_type pos = rate.find('.');
	if (pos != std::string::npos && pos + 1 == rate.length())
		return (false);
	char *end = NULL;
	errno = 0;
	double v = std::strtod(rate.c_str(), &end);
	if (end == rate.c_str() || *end != '\0')
		return (false);
	if (errno == ERANGE)
		return (false);
	if (v < 0.0 || v > FLT_MAX)
		return (false);
	return (true);
}

bool BitcoinExchange::Parsing::isValidLineDatabaseFile(const std::string &line)
{
	std::string date;
	std::string rate;

	if (line.length() < 12)
		return (false);
	date = line.substr(0, 10);
	if (!BitcoinExchange::Parsing::isValidDate(date))
		return (false);
	if (line.at(10) != ',')
		return (false);
	rate = line.substr(11);
	if (!BitcoinExchange::Parsing::isValidRate(rate))
		return (false);
	return (true);
}

bool BitcoinExchange::Parsing::isValidLineInputFile(const std::string &line)
{
	std::string date;
	std::string nb;

	if (line.length() < 13)
		return (false);
	date = line.substr(0, 10);
	if (!BitcoinExchange::Parsing::isValidDate(date))
		return (false);
	if (line.at(10) != ' ' || line.at(11) != '|' || line.at(12) != ' ')
		return (false);
	nb = line.substr(13);
	if (!BitcoinExchange::Parsing::isValidNumber(nb))
		return (false);
	return (true);
}

bool BitcoinExchange::Parsing::isValidDatabaseFile(const std::string &nameFile)
{
	int i;

	i = 1;
	std::ifstream file(nameFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("file_not_open");
	std::string line;
	while (std::getline(file, line))
	{
		if (i != 1 && !BitcoinExchange::Parsing::isValidLineDatabaseFile(line))
		{
			std::ostringstream oss;
			oss << "Database line " << i << " is not valid" << std::endl;
			std::string mess = oss.str();
			throw std::runtime_error(mess);
		}
		i++;
	}
	file.close();
	return (true);
}

std::map<std::string, float> BitcoinExchange::Parsing::parseDatabase(const std::string &nameFile)
{
	std::ifstream file(nameFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: database file is not open");
	try
	{
		BitcoinExchange::Parsing::isValidDatabaseFile(nameFile);
	}
	catch (std::exception &e)
	{
		std::string err = e.what();
		if (err == "file_not_open")
			throw std::runtime_error("Error: database file is not open");
		else
			throw;
	}
	std::string line;
	std::map<std::string, float> datas;
	int i = 0;
	while (std::getline(file, line))
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		std::string date = BitcoinExchange::Parsing::getDateInDatabase(line);
		float rate = BitcoinExchange::Parsing::getRateInDatabase(line);
		datas[date] = rate;
	}
	file.close();
	return (datas);
}
float BitcoinExchange::searchValtoLowerDate(std::string date)
{
	std::map<std::string, float>::iterator it;

	it = this->_databases.begin();
	while (it != _databases.end() && it->first < date)
	{
		it++;
	}
	if (it == _databases.begin())
		throw std::runtime_error("Error: date too old for database.");
	it--;
	return (it->second);
}

void BitcoinExchange::convertAllInputLine(std::string inputFile)
{
	std::ifstream file(inputFile.c_str());
	int i = 0;
	if (!file.is_open())
		throw std::runtime_error("Error: input file is not open");
	std::string line;
	while (std::getline(file, line))
	{
		try
		{
			if (i == 0)
			{
				i++;
				continue;
			}
			if (!BitcoinExchange::Parsing::isValidLineInputFile(line))
			{
				std::cout << "Error: bad input => " << line << std::endl;
				i++;
				continue;
			}
			std::string dateInputFile = BitcoinExchange::Parsing::getDateInInputFile(line);
			float nb = BitcoinExchange::Parsing::getNbInInputFile(line);
			std::map<std::string, float>::iterator it = _databases.find(dateInputFile);
			if (it != _databases.end())
				std::cout << dateInputFile << " => " << nb << " = " << nb * it->second << std::endl;
			else
			{
				float valInDate = searchValtoLowerDate(dateInputFile);
				std::cout << dateInputFile << " => " << nb << " = " << nb * valInDate << std::endl;
			}
			i++;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

std::string BitcoinExchange::Parsing::getDateInDatabase(const std::string &line)
{
	std::string date;

	date = line.substr(0, 10);
	return (date);
}

std::string BitcoinExchange::Parsing::getDateInInputFile(const std::string &line)
{
	std::string date;

	date = line.substr(0, 10);
	return (date);
}

float BitcoinExchange::Parsing::getNbInInputFile(const std::string &line)
{
	std::string valStr;
	float val;
	char *endPtr = 0;
	errno = 0;

	valStr = line.substr(13);
	val = static_cast<float>(std::strtod(valStr.c_str(), &endPtr));
	if (endPtr == valStr.c_str() || *endPtr != '\0')
		throw std::runtime_error("invalid_number");
	if (errno == ERANGE)
		throw std::runtime_error("overflow_float");
	return (val);
}

float BitcoinExchange::Parsing::getRateInDatabase(const std::string &line)
{
	std::string valStr;
	float val;
	char *endPtr = 0;
	errno = 0;

	valStr = line.substr(11);
	val = static_cast<float>(std::strtod(valStr.c_str(), &endPtr));
	if (endPtr == valStr.c_str() || *endPtr != '\0')
		throw std::runtime_error("invalid_number");
	if (errno == ERANGE)
		throw std::runtime_error("overflow_float");
	return (val);
}

void BitcoinExchange::convertData(std::string databaseFile, std::string inputFile)
{
	_databases = BitcoinExchange::Parsing::parseDatabase(databaseFile);
	convertAllInputLine(inputFile);
}
