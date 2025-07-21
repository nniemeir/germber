SRC := $(wildcard src/*/*.c)

NAME = germber

DESTDIR = /usr/bin/

CC = gcc

BIN_DIR = bin

BUILD_DIR = build

OBJS = $(SRC:src/%.c=$(BUILD_DIR)/%.o)

CFLAGS = -Wall -Wextra -pedantic -g -I include

LDFLAGS := $(shell sdl2-config --libs) -lSDL2_ttf -lpthread
CFLAGS += $(shell sdl2-config --cflags)

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

install: $(BIN_DIR)/$(NAME) 
	cp -f -r $(BIN_DIR)/$(NAME) $(DESTDIR)

re: fclean all

uninstall: $(NAME)
	rm -f $(DESTDIR)$(NAME)

.PHONY: all fclean install re uninstall
