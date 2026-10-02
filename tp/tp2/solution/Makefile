# Lab 2 - Multithreaded programming with OpenMP: reference solutions
#
#   make                 build all the solutions into build/
#   make run             build and run all of them
#   make run-hello       build and run one of them (run-sum, run-mergesort,
#                        run-mergesort-8threads)
#   make clean           remove build/
#
# The compiler is the most recent GNU g++ found on the machine (on macOS,
# the Homebrew one, since Apple's g++ is clang++ without OpenMP); override
# with  make CXX=g++-16  if needed. (":=" rather than "?=", because make has a
# built-in default for CXX, "c++" on macOS, that "?=" would not override.)

CXX      := $(shell command -v g++-16 || command -v g++-15 || command -v g++-14 || echo g++)
CXXFLAGS ?= -O2 -fopenmp -Wall
BUILD    := build

PROGRAMS := hello sum mergesort mergesort-8threads

all: $(addprefix $(BUILD)/,$(PROGRAMS))

$(BUILD)/%: %.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

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
