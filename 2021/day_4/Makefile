CC = gcc
CFLAGS = -g -Wall -Werror

all: bingo_player.exe

bingo_player.exe: bingo_player.c board_list.o
	$(CC) $(CFLAGS) -o $@ $^

board_list.o: board_list.c board_list.h
	$(CC) $(CFLAGS) -c board_list.c

clean:
	del /f *.o StatusAtEnd.txt