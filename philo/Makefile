NAME	=	philo
SRCS	=	philosophers.c \
			sim.c \
			eat_sleep_die.c \
			parse.c

ODIR	= obj
OBJS    = $(addprefix $(ODIR)/, $(SRCS:.c=.o))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

IFLAGS	= -Iincludes
LEAKFLAGS = -LLeakSanitizer -llsan -lc++

print-%: 
	@echo $* = $($*)

CC = clang
			
all: $(NAME)

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(ODIR)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(IFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJDIR)/$(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

debug: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(IFLAGS) -o fractol -fsanitize=address $(LEAKFLAGS)

.SUFFIXES = 
