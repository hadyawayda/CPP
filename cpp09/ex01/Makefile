# ex01/Makefile
NAME    = RPN
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -I includes

SRC_DIR = src
OBJ_DIR = obj

SRCS    = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/RPN.cpp

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
	@valgrind -q --leak-check=full --show-leak-kinds=all ./$(NAME) "8 9 * 9 - 9 - 9 - 4 - 1 +"

re: fclean all

.PHONY: all clean fclean re
