# ──────────────────────────────────────────────────────────────────────────────
# ex03 – Interface & recap
# ──────────────────────────────────────────────────────────────────────────────

INCDIR   := includes
OBJDIR   := objs
NAME     := materia

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -I $(INCDIR)
RM       := rm -rf

# Only .cpp sources here — do NOT list headers
SRCS     := AMateria.cpp Ice.cpp Cure.cpp \
           Character.cpp MateriaSource.cpp main.cpp
OBJS     := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Pattern rule to compile each .cpp into objs/*.o
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure the object directory exists
$(OBJDIR):
	@mkdir -p $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
