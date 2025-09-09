# ex02/Makefile
NAME    = PmergeMe
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -I includes

SRC_DIR = src
OBJ_DIR = obj

SRCS    = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/PmergeMe.cpp

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
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 3 5 9 7 4

test: $(NAME)
	@bash tests/run_samples.sh

fuzz: $(NAME)
	@ITER=$(ITER) MIN_N=$(MIN_N) MAX_N=$(MAX_N) MAX_VAL=$(MAX_VAL) bash tests/fuzz.sh

re: fclean all

.PHONY: all clean fclean re
