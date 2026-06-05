#ifndef PMERGEMME_HPP
#define PMERGEMME_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <cerrno>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <sys/time.h>

class PmergeMe
{
private:
	std::vector<int> _vec;
	std::deque<int> _deq;
	std::string _beforeStr;
	double _timeVec;
	double _timeDeq;

	void fillVec(int ac, char **av);
	void fillDeq(int ac, char **av);

	void sortChunkVector(size_t size);
	void makePairsVector(size_t size);
	void splitWinnersLosersVector(size_t size, std::vector<int> &winners, std::vector<int> &losers, int &last, bool &hasLast);
	void insertLosersVector(std::vector<int> &winners, std::vector<int> &losers, int last, bool hasLast);

	void sortChunkDeque(size_t size);
	void makePairsDeque(size_t size);
	void splitWinnersLosersDeque(size_t size, std::deque<int> &winners, std::deque<int> &losers, int &last, bool &hasLast);
	void insertLosersDeque(std::deque<int> &winners, std::deque<int> &losers, int last, bool hasLast);

public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	PmergeMe(int ac, char **av);
	~PmergeMe();

	static bool parse(int ac, char **av);
	void sortVector(int ac, char **av);
	void sortDeque(int ac, char **av);
	void display() const;
};

#endif
