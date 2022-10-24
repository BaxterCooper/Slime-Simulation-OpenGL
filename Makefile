all: compile

compile:
	g++ src/glad.cpp src/main.cpp -o main -I include/ -L lib -lglfw3 -lopengl32 -lglu32 -lgdi32