# Session 5 - Introduction to GPU programming with CUDA: the example programs
# of the slides (cm5-gpu-intro)
#
#   make                          build both examples into build/
#   make run                      build and run both, in the order of the slides
#   make run-02-hello-ids         hello world with thread ids
#   make run-multiply-by-two-steps  the MultiplyByTwo example, step by step
#   make clean                    remove build/
#
# Needs nvcc and an NVIDIA GPU. Without them, every example is a single file
# that runs on https://cuda.godbolt.org (see the "Compiling and running" slide).

NVCC      ?= nvcc
NVCCFLAGS ?= -O2
BUILD     := build

# in the order of the slides
PROGRAMS := 02-hello-ids multiply-by-two-steps

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
