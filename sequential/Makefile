CC = gcc
CFLAGS = -W -Wall -g

BIN_DIR := ./bin
SRC_DIR := ./src
TARGET := $(BIN_DIR)/sequential.out

# Look for .c source file in every subdir of src/
vpath %.c $(shell find $(SRC_DIR) -type d)
#SRCS contains the names of all source files under src/
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -printf "%f")
#HDRS contains the paths of all header files under src/
HDRS := $(shell find $(SRC_DIR) -name '*.h')
#OBJ contains the paths of all object files to be created under bin/
OBJS := $(patsubst %.c,$(BIN_DIR)/%.o,$(SRCS))

$(TARGET): $(OBJS) $(HDRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

#Every bin/%.o file is compiled from the matching %.c in src/ plus header files
$(BIN_DIR)/%.o: %.c $(HDRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)