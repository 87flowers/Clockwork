CXX ?= g++
ENV_CXXFLAGS := $(CXXFLAGS)
CXXFLAGS := $(ENV_CXXFLAGS) -std=c++17 -march=native -fno-exceptions -fno-rtti
RELFLAGS := $(CXXFLAGS) -DNDEBUG -O3 -flto
DEBFLAGS := $(CXXFLAGS) -O1 -g
EXE ?= clockwork

BUILD_DIR := ./build

ifeq ($(OS), Windows_NT)
    SUFFIX := .exe
else
    SUFFIX :=
endif

SOURCES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
BASESOURCES := $(filter-out src/main.cpp,$(SOURCES))

TESTS := $(patsubst tests/%.cpp,build/test_%,$(TEST_SRCS))

all: $(EXE)

test: $(TESTS)
	for t in $(TESTS); do echo "Running" $$t && $$t > /dev/null || exit 1; done

bench: $(EXE)
	./$(EXE) bench

$(EXE):
	$(CXX) $(RELFLAGS) $(SOURCES) -o $(EXE)

$(TESTS): build/test_%:
	$(CXX) $(DEBFLAGS) $(BASESOURCES) $(patsubst build/test_%,tests/%.cpp,$@) -o $@

.PHONY: all test bench $(EXE) $(TESTS)
