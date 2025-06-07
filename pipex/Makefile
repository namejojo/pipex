LIB_DIR=my_libft/
GNL_DIR=my_libft/get_next_line/

SRC_FILES=  $(wildcard ${LIB_DIR}*.c) \
			$(wildcard ${GNL_DIR}*.c)

OBJ_FILES=${SRC_FILES:.c=.o}

SRC_BONUS=$(wildcard ${BONUS_DIR}*.c)

OBJ_BONUS=${SRC_BONUS:.c=.o}

NAME=libft.a

HDR_FILE=libft.h

CC=cc

CFLAGS= -Wall -Wextra -Werror -g -s

all: beautiful ${NAME}

beautiful:
	clear

pipex: ${NAME} pipex.c
	${CC} ${CFLAGS} pipex.c ${NAME} -o pipex

lib: ${NAME} ${OBJ_FILES}
	@ar rcs $@ $?
	@echo "only library created, obj_files deleted"
	@rm -f $(OBJ_BONUS) $(OBJ_FILES) 
	@rm -f .bonus

${NAME}: ${OBJ_FILES}
	@ar rcs $@ $?
	@echo "library created"

bonus: .bonus

.bonus: ${OBJ_FILES} $(OBJ_BONUS)
	ar rcs ${NAME} $?
	@touch .bonus
	@echo "bonus library created"
	
clean: 
	@rm -f $(OBJ_BONUS) $(OBJ_FILES) 
	@rm -f .bonus
	@echo "files removed"

fclean: clean 
	@rm -f ${NAME}
	@rm -f .lib
	@echo "files really removed"

re: fclean all

.PHONY: re fclean clean all

# so:
#	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRC_FILES)
#	gcc -nostartfiles -shared -o libft.so $(OBJ_FILES)
# 