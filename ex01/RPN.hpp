#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>

class Parsing
{
private:
	Parsing();
	Parsing(const Parsing &other);
	Parsing &operator=(const Parsing &other);
	~Parsing();
	class ParseError : public std::exception
	{
	public:
		const char *what() const throw();
	};
	static std::string parse(char *av);
};

class Rpn
{
private:
	std::stack<long> _datas;

public:
	Rpn();
	Rpn(const Rpn &other);
	Rpn &operator=(const Rpn &other);

	long execRpn(std::string);
};

#endif
