CC = gcc
CFLAGS = -W -Wall -g

BIN_DIR := ./bin
SRC_DIR := ./src
TARGET := $(BIN_DIR)/sequential.out

SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c')
HDRS := $(shell find $(SRC_DIR) -name '*.h')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

$(TARGET): $(OBJS) $(HDRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

#Every .o file is compiled from the corresponding .c plus header files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)