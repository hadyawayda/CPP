# ──────────────────────────────────────────────────────────────────────────────
# ex00 – My First Class in Orthodox Canonical Form
# ──────────────────────────────────────────────────────────────────────────────
OBJDIR  := objs
NAME    := fixed0

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -I includes
RM       := rm -rf

SRCS     := main.cpp Fixed.cpp
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
