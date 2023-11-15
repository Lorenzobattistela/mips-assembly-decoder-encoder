CC = gcc
CFLAGS = -Wall -g
SRC = encoder.c decoder.c helpers.c io.c main.c
OBJS = $(SRC:.c=.o)
EXECUTABLE = converter

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

.PHONY: all run clean
