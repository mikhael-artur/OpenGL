all : car_with_lighting.cpp glad.c stb_image.cpp
	g++ car_with_lighting.cpp glad.c stb_image.cpp -lassimp -lopengl32 -lglfw3 -std=c++11