# Lab 4 - Distributed-memory programming with MPI: reference solutions
#
#   make                 build all the solutions into build/
#   make run             build and run all of them with NP processes (4 by default)
#   make run-hello       build and run one of them (run-hello, run-convolution, run-mergesort)
#   make NP=8 run-...    run with 8 processes (mergesort: a power of two)
#   make clean           remove build/
#
# Needs an MPI library (Open MPI or MPICH): mpic++ compiles, mpirun launches
# the processes. Add --oversubscribe to MPIRUN for more processes than cores:
#   make MPIRUN="mpirun --oversubscribe" NP=8 run-mergesort

MPICXX   ?= mpic++
CXXFLAGS ?= -O2 -Wall
MPIRUN   ?= mpirun
NP       ?= 4
BUILD    := build

PROGRAMS := hello convolution mergesort

all: $(addprefix $(BUILD)/,$(PROGRAMS))

$(BUILD)/%: %.cpp | $(BUILD)
	$(MPICXX) $(CXXFLAGS) $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(addprefix run-,$(PROGRAMS))

run-%: $(BUILD)/%
	@echo "=== $* ($(NP) processes) ==="
	@$(MPIRUN) -np $(NP) ./$<
	@echo

clean:
	rm -rf $(BUILD)

.PHONY: all run clean
