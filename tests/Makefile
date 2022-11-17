#Author: Antonio Langella

TARGETS  := $(shell find . -mindepth 1 -maxdepth 1 -type d)

all: $(TARGETS)
	@echo Build completed

.PHONY: $(TARGETS)
$(TARGETS):
	@echo Building $@...
	@cd $@ && $(MAKE)