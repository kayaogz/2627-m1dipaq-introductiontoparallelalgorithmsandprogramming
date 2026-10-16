# Lab 3 - GPU programming with CUDA: reference solutions
#
#   make                 build all the solutions into build/
#   make run             build and run all of them
#   make run-hello       build and run one of them (run-hello, run-saxpy)
#   make clean           remove build/
#
# Needs nvcc and an NVIDIA GPU. Without them, every file is a single
# self-contained program that runs on https://cuda.godbolt.org (language
# CUDA C++, compiler NVCC, an executor with -O2).

NVCC      ?= nvcc
NVCCFLAGS ?= -O2
BUILD     := build

PROGRAMS := hello saxpy

all: $(addprefix $(BUILD)/,$(PROGRAMS))

$(BUILD)/%: %.cu | $(BUILD)
	$(NVCC) $(NVCCFLAGS) $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(addprefix run-,$(PROGRAMS))

run-%: $(BUILD)/%
	@echo "=== $* ==="
	@./$<
	@echo

clean:
	rm -rf $(BUILD)

.PHONY: all run clean
