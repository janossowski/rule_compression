# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic

# Linker flags to force console app (prevents WinMain error)
LDFLAGS = -mconsole

# Source files
SRCS = main.cpp rule_eval.cpp rule_parser.cpp dataset_loader.cpp
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = rule_compress.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) $(TARGET) 2>nul || rm -f $(OBJS) $(TARGET)
