#include "RPN.hpp"

Parsing::Parsing() {}
Parsing::Parsing(const Parsing &other) {}
Parsing &Parsing::operator=(const Parsing &other) {}

const char *Parsing::ParseError::what() const throw()
{
	return "Error invalid string for rpn";
}

std::string Parsing::parse(char *av)
{
	std::string validChar = "0123456789*+-/";
	if (!av || !*av)
		throw Parsing::ParseError();
	std::string str = av;
	std::string::iterator it;
	while (it != str.end())
	{
		if (!validChar.find(*it))
			throw Parsing::ParseError();
		it++;
	}
	return (str);
}
