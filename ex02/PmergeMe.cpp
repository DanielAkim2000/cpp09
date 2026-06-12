#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _timeVec(0), _timeDeq(0) {}

PmergeMe::PmergeMe(const PmergeMe &other)
	: _vec(other._vec), _deq(other._deq), _beforeStr(other._beforeStr),
	  _timeVec(other._timeVec), _timeDeq(other._timeDeq) {}

PmergeMe::PmergeMe(int ac, char **av) : _timeVec(0), _timeDeq(0)
{
	std::ostringstream oss;
	for (int i = 1; i < ac; i++)
	{
		oss << av[i];
		if (i + 1 < ac)
			oss << " ";
	}
	_beforeStr = oss.str();
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
		_beforeStr = other._beforeStr;
		_timeVec = other._timeVec;
		_timeDeq = other._timeDeq;
	}
	return (*this);
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::parse(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
	{
		std::string str(av[i]);
		char *end = NULL;
		errno = 0;
		double d = std::strtod(str.c_str(), &end);
		if (end == str.c_str() || *end != '\0')
		{
			std::cerr << "Error: invalid input: " << str << std::endl;
			return (false);
		}
		if (errno == ERANGE || d < 0 || d > INT_MAX)
		{
			std::cerr << "Error: out of range: " << str << std::endl;
			return (false);
		}
	}
	return (true);
}

void PmergeMe::fillVec(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
	{
		std::string str(av[i]);
		char *end = NULL;
		double d = std::strtod(str.c_str(), &end);
		_vec.push_back(static_cast<int>(d));
	}
}

void PmergeMe::fillDeq(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
	{
		std::string str(av[i]);
		char *end = NULL;
		double d = std::strtod(str.c_str(), &end);
		_deq.push_back(static_cast<int>(d));
	}
}

void PmergeMe::makePairsVector(size_t size)
{
	for (size_t i = 0; i + 1 < size; i += 2)
	{
		if (_vec[i] < _vec[i + 1])
			std::swap(_vec[i], _vec[i + 1]);
	}
}

void PmergeMe::splitWinnersLosersVector(size_t size, std::vector<int> &winners, std::vector<int> &losers, int &last, bool &hasLast)
{
	for (size_t i = 0; i + 1 < size; i += 2)
	{
		winners.push_back(_vec[i]);
		losers.push_back(_vec[i + 1]);
	}
	hasLast = (size % 2 != 0);
	if (hasLast)
		last = _vec[size - 1];

	size_t idx = 0;
	for (size_t i = 0; i < winners.size(); i++)
		_vec[idx++] = winners[i];
	for (size_t i = 0; i < losers.size(); i++)
		_vec[idx++] = losers[i];
}

void PmergeMe::generateJacobsthalVector(std::vector<size_t> &jacobsthal, size_t losersSize) const
{
	jacobsthal.push_back(1);
	jacobsthal.push_back(3);
	while (jacobsthal.back() < losersSize)
	{
		size_t n = jacobsthal.size();
		size_t next = jacobsthal[n - 1] + 2 * jacobsthal[n - 2];
		jacobsthal.push_back(next);
	}
}

void PmergeMe::generateOrderVector(std::vector<size_t> &order, size_t losersSize, std::vector<size_t> jacobsthal) const
{
	size_t prev = 0;
	for (size_t gi = 0; gi < jacobsthal.size(); gi++)
	{
		size_t end = std::min(jacobsthal[gi], losersSize);
		for (size_t k = end; k > prev; k--)
		{
			if (k - 1 < losersSize)
				order.push_back(k - 1);
		}
		prev = end;
		if (prev >= losersSize)
			break;
	}
}

void PmergeMe::insertLosersVector(std::vector<int> &winners, std::vector<int> &losers, int last, bool hasLast)
{
	std::vector<int> mainChain(_vec.begin(), _vec.begin() + winners.size());
	std::vector<size_t> jacobsthal;
	std::vector<size_t> order;
	generateJacobsthalVector(jacobsthal, losers.size());
	generateOrderVector(order, losers.size(), jacobsthal);
	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t li = order[oi];
		std::vector<int>::iterator winPos = std::find(mainChain.begin(), mainChain.end(), winners[li]);
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), winPos + 1, losers[li]);
		mainChain.insert(pos, losers[li]);
	}
	if (hasLast)
	{
		std::vector<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), last);
		mainChain.insert(pos, last);
	}
	for (size_t i = 0; i < mainChain.size(); i++)
		_vec[i] = mainChain[i];
}

void PmergeMe::sortChunkVector(size_t size)
{
	if (size <= 1)
		return;

	makePairsVector(size);
	std::vector<int> winners;
	std::vector<int> losers;
	int last = -1;
	bool hasLast = false;
	splitWinnersLosersVector(size, winners, losers, last, hasLast);
	sortChunkVector(winners.size());
	insertLosersVector(winners, losers, last, hasLast);
}

void PmergeMe::makePairsDeque(size_t size)
{
	for (size_t i = 0; i + 1 < size; i += 2)
	{
		if (_deq[i] < _deq[i + 1])
			std::swap(_deq[i], _deq[i + 1]);
	}
}

void PmergeMe::splitWinnersLosersDeque(size_t size, std::deque<int> &winners, std::deque<int> &losers, int &last, bool &hasLast)
{
	for (size_t i = 0; i + 1 < size; i += 2)
	{
		winners.push_back(_deq[i]);
		losers.push_back(_deq[i + 1]);
	}
	hasLast = (size % 2 != 0);
	if (hasLast)
		last = _deq[size - 1];

	size_t idx = 0;
	for (size_t i = 0; i < winners.size(); i++)
		_deq[idx++] = winners[i];
	for (size_t i = 0; i < losers.size(); i++)
		_deq[idx++] = losers[i];
}

void PmergeMe::generateJacobsthalDeque(std::deque<size_t> &jacobsthal, size_t losersSize) const
{
	jacobsthal.push_back(1);
	jacobsthal.push_back(3);
	while (jacobsthal.back() < losersSize)
	{
		size_t n = jacobsthal.size();
		size_t next = jacobsthal[n - 1] + 2 * jacobsthal[n - 2];
		jacobsthal.push_back(next);
	}
}

void PmergeMe::generateOrderDeque(std::deque<size_t> &order, size_t losersSize, std::deque<size_t> jacobsthal) const
{
	size_t prev = 0;
	for (size_t gi = 0; gi < jacobsthal.size(); gi++)
	{
		size_t end = std::min(jacobsthal[gi], losersSize);
		for (size_t k = end; k > prev; k--)
		{
			if (k - 1 < losersSize)
				order.push_back(k - 1);
		}
		prev = end;
		if (prev >= losersSize)
			break;
	}
}

void PmergeMe::insertLosersDeque(std::deque<int> &winners, std::deque<int> &losers, int last, bool hasLast)
{
	std::deque<int> mainChain(_deq.begin(), _deq.begin() + winners.size());
	std::deque<size_t> jacobsthal;
	std::deque<size_t> order;
	generateJacobsthalDeque(jacobsthal, losers.size());
	generateOrderDeque(order, losers.size(), jacobsthal);
	for (size_t oi = 0; oi < order.size(); oi++)
	{
		size_t li = order[oi];
		std::deque<int>::iterator winPos = std::find(mainChain.begin(), mainChain.end(), winners[li]);
		std::deque<int>::iterator pos =
			std::lower_bound(mainChain.begin(), winPos + 1, losers[li]);
		mainChain.insert(pos, losers[li]);
	}
	if (hasLast)
	{
		std::deque<int>::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), last);
		mainChain.insert(pos, last);
	}

	for (size_t i = 0; i < mainChain.size(); i++)
		_deq[i] = mainChain[i];
}

void PmergeMe::sortChunkDeque(size_t size)
{
	if (size <= 1)
		return;
	makePairsDeque(size);
	std::deque<int> winners;
	std::deque<int> losers;
	int last = -1;
	bool hasLast = false;
	splitWinnersLosersDeque(size, winners, losers, last, hasLast);
	sortChunkDeque(winners.size());
	insertLosersDeque(winners, losers, last, hasLast);
}

void PmergeMe::sortVector(int ac, char **av)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	fillVec(ac, av);
	sortChunkVector(_vec.size());
	gettimeofday(&end, NULL);
	_timeVec = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);
}

void PmergeMe::sortDeque(int ac, char **av)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	fillDeq(ac, av);
	sortChunkDeque(_deq.size());
	gettimeofday(&end, NULL);
	_timeDeq = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);
}

void PmergeMe::display() const
{
	std::cout << "Before: " << _beforeStr << std::endl;

	std::cout << "After: ";
	for (size_t i = 0; i < _vec.size(); i++)
		std::cout << _vec[i] << " ";
	std::cout << std::endl;

	std::cout << "Time to process a range of " << _vec.size()
			  << " elements with std::vector : " << _timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
			  << " elements with std::deque  : " << _timeDeq << " us" << std::endl;
}
