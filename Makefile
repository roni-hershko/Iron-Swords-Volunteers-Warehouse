CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC_DIR = src
BIN_DIR = bin
EXECUTABLE = warehouse

# Get all .cpp files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Create corresponding object file names in the bin directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

# Targets
all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean