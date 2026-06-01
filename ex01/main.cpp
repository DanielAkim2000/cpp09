#include "RPN.hpp"

int main(int ac, char **av)
{
	Rpn rpn;

	if (ac != 2)
		return (std::cerr << "Error numbers arguments is not supported" << std::endl, 1);
	rpn.execRpn(av[1]);
	return (0);
}
