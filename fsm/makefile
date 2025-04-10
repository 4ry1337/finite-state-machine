TARGET_EXEC := fsm

CXX ?= clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2
LDFLAGS :=

# Directories for source and build output.
BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.s') # Find all source files (.cpp and .s).
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) # For each source file, generate a corresponding object file under BUILD_DIR.
DEPS := $(OBJS:.o=.d) # Dependency files for header tracking.

CINC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)

