CC = gcc

CFLAGS = -Wall -Wextra -std=c17 -I.
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-3.0 gio-2.0)

TARGET = spotlight

SRC = main.c launcher/launcherActivate.c utils/updateResults.c search.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(GTK_FLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean