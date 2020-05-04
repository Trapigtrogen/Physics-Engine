#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functionality.h>
#include <glm/glm.hpp>

//glm::cross for vec2
#include<glm/gtx/exterior_product.hpp>

//Line segment between two points of an object
struct Side {
	//Starting point of the side
	glm::vec2 a;
	//Ending point of the side
	glm::vec2 b;
};

class Object {
public:
	Object();
	~Object();

	void AddForce(glm::vec2 direction, float force);
	void AddForce(float angle, float force);
	void AddTorque(float force);

	void Impulse(glm::vec2 direction, float force);
	void Impulse(float angle, float force);

	void SetVelocity(glm::vec2 _velocity = glm::vec2(0.0f,0.0f));
	void SetVelocity(float x, float y);
	
	void SetRotation(float _rotation = 0.0f);
	void SetRotationPoint(glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void SetRotationPoint(float x = 0.0f, float y = 0.0f);
	
	void SetDrag(float value = 0.005f);
	void SetAngularDrag(float value = 0.007f);
	
	void SetGravity(float _gravity = 9.81f);
	void ApplyPhysics();

	float mass = 1.0f;
	bool useGravity = true;
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 size = glm::vec2(1.0f, 1.0f);
	glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
	float rotation = 0.0f;
	bool freezeRotation = false;
	bool FreezeMovementX = false;
	bool FreezeMovementY = false;
	
	//Collision functions
	bool IsColliding(Object* other);
	//Get objects colliding with this collider
	std::vector<Object*>GetCollidingObjects() { return objects; }

private:
	void ApplyGravity();
	void applyDrag();
	void applyAngularDrag();
	void applyMovement();

	float gravity = 0.0981f;
	glm::vec2 rotatingPointOffset = glm::vec2(0.0f, 0.0f);
	float torque = 0.0f;
	float rotationSpeed = 0.0f;
	float drag = 0.005f;
	float angularDrag = 0.007f;

	//Objects colliding with this object/collider
	std::vector<Object*>objects;

	bool PointCheck(glm::vec2 p, std::vector<Side> sides);
	bool FindCollidingObject(Object * obj);
	void RemoveCollidingObject(Object * obj);
	void OnCollisionEnter(Object * obj);
	void OnCollisionExit();

	//Object points
	//Anti-clockwise
	std::vector<glm::vec2>GetPoints();
	//Object sides
	//Anti-clockwise
	std::vector<Side>GetSides();

	float stepCounter = 0.0f;
	const float physDeltaTime = 1.0f / 60.0f;

	Functionality functions;
};