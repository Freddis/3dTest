CXX = clang++
CXXFLAGS = -g -std=c++20 \
  -I/opt/homebrew/Cellar/freetype/2.13.3/include/freetype2 \
  -I/opt/homebrew/Cellar/glew/2.2.0_1/include \
  -I/opt/homebrew/Cellar/devil/1.8.0_6/include

LDFLAGS = -L/opt/homebrew/lib -lfreetype -lGLEW -lglfw -lIL -lILU -framework OpenGL

SRCS = $(wildcard *.cpp)
TARGET = bin/main

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf bin/main