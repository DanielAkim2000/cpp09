#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	std::string fileName = av[1];
	BitcoinExchange check;
	try
	{
		check.convertData("data.csv", fileName);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
}
