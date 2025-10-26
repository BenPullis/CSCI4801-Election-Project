CXX = g++
CXXFLAGS = -std=c++23 -g -Wl,-rpath,$(DEP_DIR)/lib
ROOT_DIR = ../
BUILD_DIR = ../build
HEADERS_ALL = $(shell find ../include -name '*.h' -exec dirname {} \;)
INCLUDES = $(sort ${HEADERS_ALL}) 
SOURCES = $(shell find ./ -name '*.cc')
OBJFILES = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cc=.o))

EXE_DIR = $(BUILD_DIR)/bin/executables

$(BUILD_DIR)/%.o: %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(foreach inc, $(INCLUDES), -I$(inc)) -c $< -o $@

$(EXE_DIR): $(OBJFILES)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@