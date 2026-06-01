#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <list>
#include <stack>
#include <string>

bool isOperator(char c);

class Rpn
{
private:
	std::stack<long, std::list<long> > _datas;

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

	public:
		static std::string parse(char *av);
	};

	bool isSpace(char c) const;
	bool isDigitChar(char c) const;
	bool willAddOverflow(long a, long b) const;
	bool willSubOverflow(long a, long b) const;
	bool willMulOverflow(long a, long b) const;
	bool willDivOverflow(long a, long b) const;

public:
	Rpn();
	Rpn(const Rpn &other);
	Rpn &operator=(const Rpn &other);

	void execOperator(char op);
	void addNumber(char c);
	void execRpn(char *av);
	long getTopNumber(void);
};

#endif
