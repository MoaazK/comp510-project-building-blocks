# Compiler
CPP = g++

# Compiler flags
CPP_FLAGS = -g -Wall -Wextra -std=c++11

# Include directories
INCLUDES = -I./include -I.

# Libraries
LIBS = -lGLEW -lGL -lglfw

# Source files
SRCS = main.cpp Shapes.cpp InitShader.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Shader files
SHADERS = vshader.glsl fshader.glsl vshader_phong.glsl fshader_phong.glsl

# Name of the program
TARGET = project

# Default rule
all: $(TARGET)

# Rule to compile .cpp files to object files
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the executable
$(TARGET): $(OBJS) $(SHADERS)
	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)