NAME = PmergeMe
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I.

SRCS = main.cpp PmergeMe.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp PmergeMe.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re:
	make fclean
	make all

.PHONY= all clean fclean re
