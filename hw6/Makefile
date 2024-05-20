GCC := gcc
CFLAGS := -Wall -Wextra -Werror -g
TARGET := $(wildcard *.c) 
ELF := $(TARGET:.c=.o)
EXEC := $(TARGET:.c=)

all: $(EXEC)

$(EXEC): $(ELF)
	$(GCC) $(CFLAGS) $@.o -o $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ELF) $(EXEC)
