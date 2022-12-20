# Author: Antonio Langella

TARGETS  := common sequential mpi tests tools #cuda 
BIN_DIR := ./bin
DATA_DIR := ./data
t:= a b c
all: $(TARGETS)
	@echo Build completed successfully
	$(eval TMP := $(shell find . -name "*.out" -not -path "./bin/*"))
	@$(foreach d,$(TMP),cp $(d) ./bin;)

.PHONY: $(TARGETS)
$(TARGETS):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DATA_DIR)
	@echo Building $@...
	@cd $@ && $(MAKE)

.PHONY: clean
clean:
	@rm -r $(BIN_DIR)
	@$(foreach target,$(TARGETS),cd $(target); $(MAKE) clean; cd ..;)
#	@cd $@ && $(MAKE) clean