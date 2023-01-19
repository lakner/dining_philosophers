NAME	=	philo
SRCS	=	philosophers.c \
			read_args.c \
			init_sim.c \
			sim.c \
			time.c \
			eat_sleep_die.c \
			eat.c \
			die.c \
			error.c 

ODIR	= obj
OBJS    = $(addprefix $(ODIR)/, $(SRCS:.c=.o))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= -Iincludes

print-%: 
	@echo $* = $($*)
			
all: $(NAME)

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(IFLAGS) -o $(NAME)

clean:
	rm -rf $(ODIR)/$(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all


.SUFFIXES = 
