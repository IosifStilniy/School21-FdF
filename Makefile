SRCS		=	fdf.c						\
				$(UTLS)						\
				${wildcard GetNextLine/*.c}	\

UTLS		=	color.c						\
				engine.c					\
				painter.c					\
				utils.c						\
				utilsgraph.c				\
				mapbuilder.c				\

B_SRCS		=	${wildcard *_bonus.c}		\
				$(UTLS)						\
				${wildcard GetNextLine/*.c}	\

OBJS		=	$(SRCS:.c=.o)	

B_OBJS		=	$(B_SRCS:.c=.o)	

HEADS		=	fdf.h

LIBNAME		=	libft.a

LIBPATH		=	libft

LIB			=	$(addprefix $(addsuffix /, $(LIBPATH)), $(LIBNAME))

GNL			=	GetNextLine

PRF_LIB		=	libft/PressF

MLX			=	mlx

FRAMEWORKS	=	-framework OpenGL -framework AppKit

NAME		=	fdf

B_NAME		=	checker

GCC			=	gcc -g -Wall -Wextra -Werror -MMD

RM			=	rm -f

%.o: 		%.c
			$(GCC) -c -I$(LIBPATH) -I$(PRF_LIB) -I$(MLX) -I$(GNL) $< -o $@

$(NAME):	$(HEADS) $(LIB) $(OBJS)
			$(GCC) $(OBJS) $(LIB) -L$(MLX) -l$(MLX) $(FRAMEWORKS) -o $(NAME)

all:		$(NAME)

$(LIB):		lib

lib:		
			@$(MAKE) -C $(LIBPATH)
			@$(MAKE) -C $(MLX)

bonus:		clean $(HEADS) $(B_OBJS) $(LIB)
			$(GCC) $(B_OBJS) $(LIB) -L$(MLX) -l$(MLX) $(FRAMEWORKS) -o $(NAME)

clean:
			@$(MAKE) -C $(LIBPATH) clean
			$(RM) $(wildcard *.d)
			$(RM) $(wildcard GetNextLine/*.d)
			$(RM) $(OBJS)
			$(RM) $(B_OBJS)

fclean: 	clean
			@$(MAKE) -C $(MLX) clean
			@$(MAKE) -C $(LIBPATH) fclean
			$(RM) $(NAME)
			$(RM) $(B_NAME)

re:			fclean all

.PHONY:		all bonus clean fclean re lib