#include <functionality.h>


Functionality::Functionality() {}

Functionality::~Functionality() {}

int Functionality::getRandomInt(int start, int end) {
	return start + rand() % end;
}

float Functionality::getRandomFloat(float start, int end) {
	return start + rand() % end;
}

std::string Functionality::loadFile(const std::string fileName) {
	std::ifstream file;
	file.open(fileName);
	std::string buffer;
	std::getline(file, buffer, (char)file.eof());
	file.close();
	return buffer;
}

void Functionality::writeFile(const std::string fileName, std::string string) {
	std::ofstream file(fileName);
	file << string;
	file.close();
}

double Functionality::DeltaTime() {
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	return deltaTime;
}
