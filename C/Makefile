CC = gcc
CFLAGS = -Wall -Wextra
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-3.0 gio-2.0)

spotlight: spotlight.c
	$(CC) $(CFLAGS) spotlight.c -o spotlight $(GTK_FLAGS)

run: spotlight
	./spotlight

clean:
	rm -f spotlight
