# ──────────────────────────────────────────────────────────────────────────────
# PROJECT DIRECTORIES
# ──────────────────────────────────────────────────────────────────────────────
OBJDIR			:= objs

# ──────────────────────────────────────────────────────────────────────────────
# BUILD SETTINGS
# ──────────────────────────────────────────────────────────────────────────────
NAME			:= tests
CXX				:= c++
RM				:= @rm -rf
CXXFLAGS		:= -Wall -Wextra -Werror -std=c++98 -I includes

# ──────────────────────────────────────────────────────────────────────────────
# SOURCE & OBJECT LISTS
# ──────────────────────────────────────────────────────────────────────────────
SRCS			:= tests.cpp Account.cpp
OBJS			:= $(SRCS:%.cpp=$(OBJDIR)/%.o)

# ──────────────────────────────────────────────────────────────────────────────
# DEFAULT TARGET
# ──────────────────────────────────────────────────────────────────────────────
all: $(NAME)

# ──────────────────────────────────────────────────────────────────────────────
# LINKING
# ──────────────────────────────────────────────────────────────────────────────
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# ──────────────────────────────────────────────────────────────────────────────
# COMPILE RULE
# ──────────────────────────────────────────────────────────────────────────────
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ──────────────────────────────────────────────────────────────────────────────
# CLEANUP
# ──────────────────────────────────────────────────────────────────────────────
clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
