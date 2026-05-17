#include "BitcoinExchange.hpp"

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

BitcoinExchange::BitcoinExchange() : _inputs() {}

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

	it = nb.begin();
	while (it != nb.end())
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)) && *it != '.')
			return (false);
		if (*it == '.')
		{
			if (nbComma == 1)
				return (false);
			nbComma++;
		}
		it++;
	}
	std::string::size_type pos = nb.find('.');
	if (pos != std::string::npos && pos + 1 == nb.length())
		return (false);
	char *end = NULL;
	errno = 0;
	double v = std::strtod(nb.c_str(), &end);
	if (end == nb.c_str() || *end != '\0')
		return (false);
	if (errno == ERANGE)
		return (false);
	if (v > FLT_MAX)
		return (false);
	if (v < 0.0 || v > 1000.0)
		return (false);
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
	rate = line.substr(12);
	if (!BitcoinExchange::Parsing::isValidRate(rate))
		return (false);
	return (true);
}

bool BitcoinExchange::Parsing::isValidLineInputFile(const std::string &line)
{
	std::string date;
	std::string nb;

	if (line.length() < 12)
		return (false);
	date = line.substr(0, 10);
	if (!BitcoinExchange::Parsing::isValidDate(date))
		return (false);
	if (line.at(10) != ',')
		return (false);
	nb = line.substr(12);
	if (!BitcoinExchange::Parsing::isValidNumber(nb))
		return (false);
	return (true);
}
