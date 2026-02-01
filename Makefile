CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
WWW_DIR = www

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/http_handler.c $(SRC_DIR)/file_server.c $(SRC_DIR)/mime_types.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = httpserver

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build Complete: $(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Clean complete"

rebuild: clean all

.PHONY: all clean rebuild run

