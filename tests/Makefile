#Author: Antonio Langella

TARGETS  := test1 test2
BIN_DIR := ./bin

all: $(TARGETS)
	@echo Build started

.PHONY: $(TARGETS)
$(TARGETS):
	@mkdir -p $(BIN_DIR)
	@echo Building $@...
	@cd $@ && $(MAKE)