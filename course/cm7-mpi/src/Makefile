# Session 7 - Distributed-memory programming with MPI: the example programs of the slides
#
#   make                    build all the examples into build/
#   make run                build and run all of them, in the order of the slides
#   make run-02-nine-rank   build and run one of them (run-01-nine-seq, run-02-nine-rank,
#                           ..., run-05-normalize: the numbers follow the slides)
#   make NP=4 run-...       run with 4 processes instead of 3
#   make clean              remove build/
#
# Needs an MPI library (Open MPI or MPICH): mpic++ compiles, mpirun launches
# the processes. 01-nine-seq.cpp has no MPI call: mpirun just launches
# NP copies of it.

MPICXX   ?= mpic++
CXXFLAGS ?= -O2 -Wall
MPIRUN   ?= mpirun
NP       ?= 3
BUILD    := build

# numbered in the order of the slides
PROGRAMS := 01-nine-seq 02-nine-rank 03-nine-merge 04-nine-bcast 05-normalize

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
