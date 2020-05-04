#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <GLFW/glfw3.h> 

class Functionality {
public:
	Functionality();
	~Functionality();

	int getRandomInt(int start, int end);
	float getRandomFloat(float start, int end);

	double DeltaTime();

	std::string loadFile(const std::string fileName);
	void writeFile(const std::string fileName, const std::string string);
private:
	double deltaTime = 0.0f;	// Time between current frame and last frame
	double lastFrame = 0.0f; // Time of last frame
};