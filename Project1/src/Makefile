CXX = g++
CXXFLAGS = -std=c++23 -g -Wl,-rpath,$(DEP_DIR)/lib
ROOT_DIR = ../
BUILD_DIR = ../build
INCLUDE_DIR = ../include
HEADERS_ALL = $(shell find $(INCLUDE_DIR) -name '*.h' -exec dirname {} \;)
INCLUDES = $(sort $(HEADERS_ALL))
SOURCES = $(shell find ./ -name '*.cc')
OBJFILES = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cc=.o))
EXE_DIR = $(BUILD_DIR)/bin/executables

$(BUILD_DIR)/%.o: %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(foreach inc, $(INCLUDES), -I$(inc)) -I$(INCLUDE_DIR) -c $< -o $@

$(EXE_DIR): $(OBJFILES)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@

test:
	mkdir -p $(BUILD_DIR)/tests
	$(CXX) $(CXXFLAGS) $(foreach inc, $(INCLUDES), -I$(inc)) -I$(INCLUDE_DIR) -I../testing \
		$(filter-out $(BUILD_DIR)/./main.o, $(OBJFILES)) \
		$(shell find ../testing -name '*.cc') \
		-o $(BUILD_DIR)/tests/runTests -lgtest -lpthread
	$(BUILD_DIR)/tests/runTests

clean:
	rm -rf $(BUILD_DIR)
