CXX = clang++
CXXFLAGS = -std=c++17 -g -I./stb $(shell pkg-config --cflags glfw3) -I /mnt/Code/libs
LDFLAGS = $(shell pkg-config --libs glfw3)
TARGET = output/abu
SOURCES = main.cpp glad.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	mkdir -p $(dir $(TARGET))
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run:
	./output/abu
