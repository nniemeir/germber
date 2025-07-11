SRC = \
src/bus.c \
src/cart.c \
src/cpu.c \
src/cpu_utils.c \
src/emu.c \
src/file.c \
src/header.c \
src/instructions.c \
src/licensees.c \
src/main.c \
src/rom.c

NAME = germber

DESTDIR = /usr/bin/

CC = gcc

BIN_DIR = bin

BUILD_DIR = build

OBJS = $(SRC:src/%.c=$(BUILD_DIR)/%.o)

CFLAGS = -Wall -Wextra -pedantic -g -I include

all: bin $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJS)
	$(CC) -o $(BIN_DIR)/$(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -g $(CFLAGS) -c $< -o $@

bin:
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(OBJS)
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

test: $(BIN_DIR)/$(NAME)
	./test/test.exp

install: $(BIN_DIR)/$(NAME) 
	cp -f -r $(BIN_DIR)/$(NAME) $(DESTDIR)

re: fclean all

uninstall: $(NAME)
	rm -f $(DESTDIR)$(NAME)

.PHONY: all fclean install re test uninstall
