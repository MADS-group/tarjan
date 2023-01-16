# Author: Antonio Langella

TARGETS  := common sequential tests tools
BIN_DIR := ./bin
DATA_DIR := ./data

TARGETS += $(if $(shell which mpicxx), common_mpi mpi, ) #add mpi targets if mpi compiler is present
TARGETS += $(if $(shell which nvcc), cuda, ) #add cuda targets if cuda compiler is present
TARGETS += $(if $(shell which nvcc && which mpicxx), cuda_mpi, ) #add cuda targets if cuda compiler is present

green := "\033[0;32m"
cyan := "\033[0;36m"
red := "\033[0;31m"
color_off := "\033[0m"

all: $(TARGETS)
	@echo ${green}Build completed successfully
	$(eval TMP := $(shell find . -name "*.out" -not -path "./bin/*"))
	@$(foreach d,$(TMP),cp $(d) ./bin;)

.PHONY: pre-build
pre-build:
ifeq (, $(shell which mpicxx))
	@echo ${red}mpicxx not found. Skipping mpi-related sources.${color_off}
endif
ifeq (, $(shell which nvcc))
	@echo ${red}nvcc not found. Skipping cuda-related sources.${color_off}
endif

.PHONY: $(TARGETS)
$(TARGETS): pre-build
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DATA_DIR)
	@echo ${cyan}Building $@...${color_off}
	@cd $@ && $(MAKE) --no-print-directory

.PHONY: clean
clean:
	@$(foreach target,$(TARGETS),cd $(target); $(MAKE) clean; cd ..;)
	@rm -r $(BIN_DIR)
#	@cd $@ && $(MAKE) clean