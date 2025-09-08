# ex00/Makefile
NAME    = btc
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -I includes

SRCS    = src/main.cpp \
		  src/BitcoinExchange.cpp
OBJS    = $(SRCS:.cpp=.o)
DEPS    = BitcoinExchange.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
