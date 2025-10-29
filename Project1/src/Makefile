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

TEST_SOURCES = $(shell find ../testing -name '*.cc')
TEST_BIN = ../test_bin
OBJFILES_NO_MAIN = $(filter-out %main.o, $(OBJFILES))

test: $(OBJFILES_NO_MAIN) $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(foreach inc, $(INCLUDES), -I$(inc)) -I/usr/include/gtest \
	$(TEST_SOURCES) $(OBJFILES_NO_MAIN) -lgtest -lgtest_main -lpthread -o $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
	rm $(TEST_BIN)