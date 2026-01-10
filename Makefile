CXX = g++
CXXFLAGS = -std=c++23 -Wall -O2

OUTPUT_DIR = bin
SRC_DIR = src
BUILD_DIR = build

TARGET = $(OUTPUT_DIR)/GHOST

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

debug: CXXFLAGS += -DDEBUG
debug: clean all

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@mkdir -p $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: ./$(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean