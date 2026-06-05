#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error: no arguments" << std::endl;
		return (1);
	}
	if (PmergeMe::parse(ac, av))
	{
		PmergeMe pm(ac, av);
		pm.sortVector(ac, av);
		pm.sortDeque(ac, av);
		pm.display();
	}

	return 0;
}
