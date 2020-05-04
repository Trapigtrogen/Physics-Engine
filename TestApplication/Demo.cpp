#include "Demo.h"

Demo::Demo() {}

Demo::~Demo() {}

std::vector<Object*> objects;

int main() {
	engine::Window window;
	engine::GraphicsSystem graphics;

	GLFWwindow* app = window.CreateWindow();
	glEnable(GL_DEPTH_TEST);
	GLuint cubeShader = graphics.CreateShader("Assets/vertexShader.txt", "Assets/fragmentShader.txt");
	int texture = graphics.loadTexture((char*)"Assets/wall.jpg");

	glm::vec2 size(1.3f, 0.3f);

	Object* cube1 = new Object;
	cube1->size = size;
	cube1->useGravity = false;
	cube1->FreezeMovementX = true;
	cube1->FreezeMovementY = true;
	cube1->freezeRotation = true;
	cube1->position = glm::vec2(0.0f, 0.0f);
	//cube1->SetRotationPoint(1.0f, 0.0f);
	//objects.push_back(cube1);
	   	 
	Object* cube2 = new Object;	
	cube2->position = glm::vec2(0.5f, 2.0f);
	cube2->size = size;
	objects.push_back(cube2);

	
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		Object *cube = new Object;
		cube->mass = 1.0f;
		glm::vec2 size(1.3f, 0.3f);
		cube->position = glm::vec2(rand() % 11 - 5, rand() % 11 - 5);
		cube->size = size;
		objects.push_back(cube);
		cube->useGravity = rand() % 2;
	}
	

	Object floor;
	glm::vec2 floorSize(15.0f, 0.3f);
	floor.size = floorSize;
	floor.position = glm::vec2(0.0f, -3.0f - floorSize.y);

	Object ceiling;
	ceiling.size = floorSize;
	ceiling.position = glm::vec2(0.0f, 3.0f + floorSize.y);

	Object right;
	glm::vec2 wallSize(0.3f, 15.0f);
	glm::vec2 rightPosition(-5.0f - size.x / 2, 0.0f);
	right.size = wallSize;
	right.position = rightPosition;

	Object left;
	glm::vec2 leftPosition(5.0f + size.x / 2, 0.0f);
	left.size = wallSize;
	left.position = leftPosition;

	float angle = 45.0;

	// game loop
	while (!glfwWindowShouldClose(app)) {
		graphics.ClearScreen(app, 0.2f, 0.2f, 0.2f);

		// Input
		window.input.ProcessInput();
		
		for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			if ((*it)->IsColliding(cube1)) {
				std::cout << "is colliding\n";
			}
		}

		if (window.input.GetKeyDown(GLFW_KEY_W)) {
			for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
				(*it)->Impulse(glm::vec2(0.0f, 1.0f), 10.0f);
			}
		}

		if (window.input.GetKeyDown(GLFW_KEY_D)) {
			for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
				(*it)->Impulse(glm::vec2(1.0f, 0.0f), 3.0f);
				(*it)->AddRotationSpeed(-15.0f);
			}
		}

		if (window.input.GetKeyDown(GLFW_KEY_A)) {
			for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
				(*it)->Impulse(glm::vec2(-1.0f, 0.0f), 3.0f);
				(*it)->AddRotationSpeed(15.0f);
			}
		}

		if (window.input.GetKeyDown(GLFW_KEY_P)) {
			for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
				(*it)->SetVelocity(glm::vec2(0.0f, 0.0f));
				(*it)->SetRotation(0.0f);
				(*it)->SetRotationSpeed(0.0f);
			}
		}
		// Apply Physics to all objects
		for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			(*it)->ApplyPhysics();
		}

		// Temp collisions
		for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			if ((*it)->position.y < -3.0f && !window.input.GetKey(GLFW_KEY_W)) {
				(*it)->position.y = -3.0f;
				(*it)->SetVelocity();
				(*it)->SetRotation(0.0f);
				(*it)->SetRotationSpeed(0.0f);
			}
			// Temp ceiling
			if ((*it)->position.y > 3.05f) {
				(*it)->position.y = 3.0f;
				(*it)->SetVelocity(glm::vec2((*it)->velocity.x, 0.0f));
				(*it)->SetRotation(0.0f);
				(*it)->SetRotationSpeed(0.0f);
			}

			// Left
			if ((*it)->position.x < -5.5f) {
				(*it)->position.x = -5.5f;
				(*it)->SetRotation(90.0f);
				(*it)->SetRotationSpeed(0.0f);
			}
			// Right
			if ((*it)->position.x > 5.5f) {
				(*it)->position.x = 5.5f;
				(*it)->SetRotation(90.0f);
				(*it)->SetRotationSpeed(0.0f);
			}
		}

		// Draw
		for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			graphics.transform(cubeShader, (*it)->rotation, (*it)->position.x, (*it)->position.y, (*it)->size.x, (*it)->size.y);
			graphics.DrawCube(cubeShader, texture);
		}

		graphics.transform(cubeShader, cube1->rotation, cube1->position.x, cube1->position.y, cube1->size.x, cube1->size.y);
		graphics.DrawCube(cubeShader, texture);

		// Temp walls
		graphics.transform(cubeShader, floor.rotation, floor.position[0], floor.position[1], floor.size[0], floor.size[1]);
		graphics.DrawCube(cubeShader, texture);
		graphics.transform(cubeShader, ceiling.rotation, ceiling.position[0], ceiling.position[1], ceiling.size[0], ceiling.size[1]);
		graphics.DrawCube(cubeShader, texture);
		graphics.transform(cubeShader, right.rotation, right.position[0], right.position[1], right.size[0], right.size[1]);
		graphics.DrawCube(cubeShader, texture);
		graphics.transform(cubeShader, left.rotation, left.position[0], left.position[1], left.size[0], left.size[1]);
		graphics.DrawCube(cubeShader, texture);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(app);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
