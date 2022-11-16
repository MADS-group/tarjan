# Author: Antonio Langella

TARGETS  := common sequential mpi cuda
BIN_DIR := ./bin

all: $(TARGETS)
	@echo Build started

.PHONY: $(TARGETS)
$(TARGETS):
	@mkdir -p $(BIN_DIR)
	@echo Building $@...
	@cd $@ && $(MAKE)

.PHONY: clean
clean:
#	@rm -r $(BIN_DIR)
	@$(foreach target,$(TARGETS),cd $(target); $(MAKE) clean; cd ..;)
#	@cd $@ && $(MAKE) clean