# ──────────────────────────────────────────────────────────────────────────────
# ex01 – I don’t want to set the world on fire
# ──────────────────────────────────────────────────────────────────────────────

OBJDIR   := objs
NAME     := animals

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -I includes
RM       := rm -rf

SRCS     := Animal.cpp Dog.cpp Cat.cpp WrongAnimal.cpp WrongCat.cpp Brain.cpp main.cpp
OBJS     := $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
