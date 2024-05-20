CC := g++
FLAGS := -std=c++20 -O3 -Wall -Wextra -Werror
EXEC := b081020008
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(FLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(FLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJ)
