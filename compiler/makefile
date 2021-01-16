CXX := g++
CXXFLAGS := -std=c++17 -Wall

OUT_DIR := out
OBJ_DIR := out/obj

MAIN_SRC := rvc.cpp
SRCS := $(filter-out $(MAIN_SRC), $(wildcard *.cpp))
TESTSRC := $(wildcard tests/*.cpp)

MAIN_OBJ := out/obj/rvc.o
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
TESTOBJ := $(TESTSRC:%.cpp=$(OBJ_DIR)/%.o)

all: $(OUT_DIR)/RVC test

$(OUT_DIR)/RVC: $(OBJS) $(MAIN_OBJ)
	$(CXX) $(MAIN_OBJ) $(OBJS) -o $@

# Probably will refactor this later
$(OUT_DIR)/ParserTests: $(TESTOBJ) $(OBJS)
	$(CXX) $(TESTOBJ) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) -I . $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/tests/%.o: tests/%.cpp
	mkdir -p $(OBJ_DIR)/tests
	$(CXX) -I . $(CXXFLAGS) -c $< -o $@

test: $(OUT_DIR)/ParserTests
	@$(OUT_DIR)/ParserTests

.PHONY: clean
clean:
	$(RM) $(OUT_DIR)/RVC $(OUT_DIR)/ParserTest $(OBJ_DIR)/*.o $(OBJ_DIR)/tests/*.o
