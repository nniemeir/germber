SRC = \
src/bus.c \
src/cart.c \
src/cpu.c \
src/cpu_fetch.c \
src/cpu_proc.c \
src/cpu_proc_alu.c \
src/cpu_proc_bit.c \
src/cpu_proc_control.c \
src/cpu_proc_interrupts.c \
src/cpu_proc_mem.c \
src/cpu_proc_misc.c \
src/cpu_utils.c \
src/dma.c \
src/dbg.c \
src/emu.c \
src/gamepad.c \
src/instructions.c \
src/interrupts.c \
src/io.c \
src/lcd.c \
src/main.c \
src/ppu.c \
src/ppu_sm.c \
src/ppu_pipeline.c \
src/ram.c \
src/stack.c \
src/timer.c \
src/ui.c

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
