# Session 3 - Multicore programming with OpenMP: the example programs of the slides
#
#   make                 build all the examples into build/
#   make run             build and run all of them, in the order of the slides
#   make run-01-hello    build and run one of them (run-01-hello, run-02-hello-ids,
#                        ..., run-10-race-fixed: the numbers follow the slides)
#   make clean           remove build/
#
# The compiler is the most recent GNU g++ found on the machine (on macOS,
# the Homebrew one, since Apple's g++ is clang++ without OpenMP); override
# with  make CXX=g++-16  if needed. (":=" rather than "?=", because make has a
# built-in default for CXX, "c++" on macOS, that "?=" would not override.)

CXX      := $(shell command -v g++-16 || command -v g++-15 || command -v g++-14 || echo g++)
CXXFLAGS ?= -O2 -fopenmp -Wall
BUILD    := build

# numbered in the order of the slides
PROGRAMS := 01-hello 02-hello-ids 03-variables 04-bcast 05-nine-seq \
            06-nine-redundant 07-nine-thid 08-nine-sections 09-race 10-race-fixed

all: $(addprefix $(BUILD)/,$(PROGRAMS))

# The two race examples are compiled without optimization, so that
# "n = n + 1" is executed as written (load, add, store) at every iteration;
# with -O2 the compiler counts in a register and stores once per thread,
# and the race shows up as whole blocks lost, or not at all.
$(BUILD)/09-race $(BUILD)/10-race-fixed: CXXFLAGS := -O0 -fopenmp -Wall

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
