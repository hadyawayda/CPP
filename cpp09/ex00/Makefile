# ex00/Makefile
NAME    = btc
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -I includes

SRC_DIR = src
OBJ_DIR = obj

SRCS    = $(SRC_DIR)/main.cpp \
		  $(SRC_DIR)/BitcoinExchange.cpp

OBJS    = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

leaks:
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) input.txt

re: fclean all

.PHONY: all clean fclean re
