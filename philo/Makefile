NAME	= fractol
SRCS	= ft_fractol.c \
		  ft_mandelbrot.c \
		  ft_julia.c \
		  complex_math.c \
		  events.c \
		  zoom.c \
		  draw.c \
		  parse.c

ODIR	= obj
OBJS    = $(addprefix $(ODIR)/, $(SRCS:.c=.o))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

LIBNAME = libft.a
LIBFTDIR = libft
LFLAGS	= -L$(LIBFTDIR) -lft -Lmlx -lmlx
IFLAGS	= -Imlx -Iincludes
FRAMEWORK =  -framework OpenGL -framework AppKit
LIBFTOBJS = ft_isalpha.o ft_isdigit.o ft_isalnum.o ft_isascii.o ft_isprint.o ft_strlen.o ft_memset.o ft_bzero.o ft_memcpy.o ft_memmove.o ft_strlcpy.o ft_strlcat.o ft_toupper.o ft_tolower.o ft_strchr.o ft_strrchr.o ft_strncmp.o ft_memchr.o ft_memcmp.o ft_strnstr.o ft_atoi.o ft_calloc.o ft_strdup.o ft_substr.o ft_strjoin.o ft_strtrim.o ft_split.o ft_itoa.o ft_strmapi.o ft_striteri.o ft_putchar_fd.o ft_putstr_fd.o ft_putendl_fd.o ft_putnbr_fd.o
LIBFTHEADERS = $(LIBFTDIR)/libft.h 
LEAKFLAGS = -LLeakSanitizer -llsan -lc++

print-%: 
	@echo $* = $($*)

CC = clang
			
all: $(NAME)
			
libmlx.dylib:
	make -C ./mlx
	cp ./mlx/libmlx.dylib .

$(LIBFTDIR)/$(LIBNAME):
	make -C $(LIBFTDIR)

$(NAME): $(OBJS) libmlx.dylib $(LIBFTDIR)/$(LIBNAME)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(FRAMEWORK) $(IFLAGS) -o $(NAME)

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c $(ODIR)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean: libftclean
	rm -rf $(OBJDIR)/$(OBJS)

fclean: clean
	rm -rf $(NAME)

libftclean:
	make -C $(LIBFTDIR) fclean

re: fclean all

debug: $(OBJS) libmlx.dylib $(LIBFTDIR)/$(LIBNAME)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(FRAMEWORK) $(IFLAGS) -o fractol -fsanitize=address $(LEAKFLAGS)

.SUFFIXES = 

.PHONY: libft