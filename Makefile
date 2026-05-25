CXX = g++
CXXFLAGS = -g -I C:/Personal/add/GLFW/include -I C:/Personal/add/GLAD/include -I C:/Personal/add/glm -I C:/Personal/add/stb
LDFLAGS = -L C:/Personal/add/GLFW/lib-mingw-w64
LIBS = -lglfw3 -luser32 -lkernel32 -lopengl32 -lgdi32

TARGET = Output/OpenGL.exe
SOURCES = main.cpp glad.c engine.hpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)

clean:
	del /Q Output\OpenGL.exe