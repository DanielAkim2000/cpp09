#include "RPN.hpp"

#include <cctype>
#include <climits>

Rpn::Parsing::Parsing() {}
Rpn::Parsing::Parsing(const Parsing &other)
{
	(void)other;
}
Rpn::Parsing &Rpn::Parsing::operator=(const Parsing &other)
{
	(void)other;
	return (*this);
}

bool isOperator(char c)
{
	std::string validChar = "*+-/";
	if (validChar.find(c) != std::string::npos)
		return (true);
	return (false);
}

const char *Rpn::Parsing::ParseError::what() const throw()
{
	return "Error invalid string for rpn";
}

std::string Rpn::Parsing::parse(char *av)
{
	if (!av || !*av)
		throw Rpn::Parsing::ParseError();
	std::string str = av;
	bool sawToken = false;
	for (size_t i = 0; i < str.size(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(str[i]);
		if (std::isspace(c))
			continue;
		if (!std::isdigit(c) && !isOperator(str[i]))
			throw Rpn::Parsing::ParseError();
		if (i + 1 < str.size() && !std::isspace(static_cast<unsigned char>(str[i + 1])))
			throw Rpn::Parsing::ParseError();
		sawToken = true;
	}
	if (!sawToken)
		throw Rpn::Parsing::ParseError();
	return (str);
}

Rpn::Rpn() : _datas() {}

Rpn::Rpn(const Rpn &other)
{
	*this = other;
}

Rpn &Rpn::operator=(const Rpn &other)
{
	if (this != &other)
	{
		_datas = other._datas;
	}
	return (*this);
}

void Rpn::addNumber(char c)
{
	long val;
	val = c - '0';
	this->_datas.push(val);
}

bool isSpace(char c)
{
	return (std::isspace(static_cast<unsigned char>(c)) != 0);
}

bool isDigitChar(char c)
{
	return (std::isdigit(static_cast<unsigned char>(c)) != 0);
}

bool Rpn::willAddOverflow(long a, long b) const
{
	if (b > 0 && a > LONG_MAX - b)
		return (true);
	if (b < 0 && a < LONG_MIN - b)
		return (true);
	return (false);
}

bool Rpn::willSubOverflow(long a, long b) const
{
	if (b < 0 && a > LONG_MAX + b)
		return (true);
	if (b > 0 && a < LONG_MIN + b)
		return (true);
	return (false);
}

bool Rpn::willMulOverflow(long a, long b) const
{
	if (a == 0 || b == 0)
		return (false);
	if (a > 0)
	{
		if (b > 0)
			return (a > LONG_MAX / b);
		return (b < LONG_MIN / a);
	}
	if (b > 0)
		return (a < LONG_MIN / b);
	return (a != 0 && b < LONG_MAX / a);
}

bool Rpn::willDivOverflow(long a, long b) const
{
	return (a == LONG_MIN && b == -1);
}

long Rpn::getTopNumber(void)
{
	long nb;
	nb = this->_datas.top();
	this->_datas.pop();
	return (nb);
}

void Rpn::execOperator(char op)
{
	if (this->_datas.size() < 2)
		throw std::runtime_error("Error bad syntax");
	long nb1;
	long nb2;
	long result;

	nb2 = this->getTopNumber();
	nb1 = this->getTopNumber();
	if (op == '/' && nb2 == 0)
		throw std::runtime_error("Error division by 0 is not supported");
	if (op == '/' && this->willDivOverflow(nb1, nb2))
		throw std::runtime_error("Error overflow");
	switch (op)
	{
	case '/':
		result = nb1 / nb2;
		break;
	case '*':
		if (this->willMulOverflow(nb1, nb2))
			throw std::runtime_error("Error overflow");
		result = nb1 * nb2;
		break;
	case '+':
		if (this->willAddOverflow(nb1, nb2))
			throw std::runtime_error("Error overflow");
		result = nb1 + nb2;
		break;
	case '-':
		if (this->willSubOverflow(nb1, nb2))
			throw std::runtime_error("Error overflow");
		result = nb1 - nb2;
		break;
	default:
		throw std::runtime_error("Error operator not found");
	}
	this->_datas.push(result);
}

void Rpn::execRpn(char *av)
{
	if (!av || !*av)
	{
		std::cerr << "Error string empty" << std::endl;
		return;
	}
	try
	{
		Rpn::Parsing::parse(av);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	std::string str = av;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (isSpace(str[i]))
			continue;
		if (isDigitChar(str[i]))
		{
			this->addNumber(str[i]);
		}
		else if (isOperator(str[i]))
		{
			try
			{
				this->execOperator(str[i]);
			}
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				return;
			}
		}
		else
		{
			std::cerr << "Error bad token" << std::endl;
			return;
		}
	}
	if (this->_datas.size() != 1)
	{
		std::cerr << "Error syntax not valid" << std::endl;
		return;
	}
	std::cout << this->_datas.top() << std::endl;
	this->_datas.pop();
}
