# Author: Antonio Langella

TARGETS  := common sequential mpi cuda tests
BIN_DIR := ./bin
DATA_DIR := ./data

all: $(TARGETS)
	@echo Build started

.PHONY: $(TARGETS)
$(TARGETS):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DATA_DIR)
	@echo Building $@...
	@cd $@ && $(MAKE)

.PHONY: clean
clean:
#	@rm -r $(BIN_DIR)
	@$(foreach target,$(TARGETS),cd $(target); $(MAKE) clean; cd ..;)
#	@cd $@ && $(MAKE) clean