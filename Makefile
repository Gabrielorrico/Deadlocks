CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

banker: src/banker.c
	$(CC) $(CFLAGS) src/banker.c -o banker

clean:
	rm -f banker

.PHONY: clean