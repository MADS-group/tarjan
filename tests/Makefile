#Author: Antonio Langella

TARGETS  := $(shell find . -mindepth 1 -maxdepth 1 -type d)

all: $(TARGETS)
	@:

.PHONY: $(TARGETS)
$(TARGETS):
	@echo Building $@...
	@cd $@ && $(MAKE)

.PHONY: clean
clean:
	@$(foreach target,$(TARGETS),cd $(target); $(MAKE) clean; cd ..;)