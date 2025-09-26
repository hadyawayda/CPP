# Test Makefile for BitcoinExchange
NAME    = test_btc
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -I ../includes -I .

SRC_DIR = .
OBJ_DIR = obj
MAIN_SRC_DIR = ../src

# Test sources
TEST_SRCS = $(SRC_DIR)/test_main.cpp \
            $(SRC_DIR)/test_BitcoinExchange.cpp

# Main project sources (exclude main.cpp)
MAIN_SRCS = $(MAIN_SRC_DIR)/BitcoinExchange.cpp

ALL_SRCS = $(TEST_SRCS) $(MAIN_SRCS)
OBJS = $(ALL_SRCS:%.cpp=$(OBJ_DIR)/%.o)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "Test executable created: $(NAME)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files cleaned"

fclean: clean
	@rm -f $(NAME)
	@rm -f *.csv *.txt  # Clean up test files
	@echo "Executable and test files cleaned"

test: $(NAME)
	@echo "Running unit tests..."
	@./$(NAME)

re: fclean all

.PHONY: all clean fclean test re
