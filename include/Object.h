#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functionality.h>
#include <glm/glm.hpp>
#include<glm/gtx/exterior_product.hpp>
#include <glm/gtx/rotate_vector.hpp>



/*
Collider taken from tatti engine and edited from there. Still not working
Everything is in one file because the engine won't let move the data I needed
*/

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

	void AddForce(glm::vec2 direction, float amount);
	void AddForce(float angle, float amount);

	void Impulse(glm::vec2 direction, float force);
	void Impulse(float angle, float force);

	void SetVelocity(glm::vec2 _velocity = glm::vec2(0.0f,0.0f));
	void SetVelocity(float x, float y);

	void SetAcceleration(glm::vec2 _acceleration);
	void SetAcceleration(float accelerationX = 0.0f, float accelerationY = 0.0f);

	void AddRotationSpeed(float force);
	void SetRotationSpeed(float _rotationSpeed = 0.0f);

	void AddTorque(float force);
	void SetTorque(float _torque = 0.0f);
	
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
	glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);;
	float rotation = 0.0f;
	bool freezeRotation = false;
	bool FreezeMovementX = false;
	bool FreezeMovementY = false;

	// Collision
	bool IsColliding(Object* other);

	//Get objects colliding with this collider
	std::vector<Object*>GetCollidingObjects() { return objects; }
	//Get point of collision
	glm::vec2 GetCollisionPoint() { return point; }
	//Get normal of the collision
	glm::vec2 GetCollisionNormal() { return normal; }

	std::vector<glm::vec2>GetPoints();
	std::vector<Side>GetSides();

private:
	std::vector<float>vertices = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	void ApplyGravity();
	void ApplyDrag();
	void ApplyAngularDrag();
	void ApplyMovement();

	float gravity = 9.81f / 10.0f;
	glm::vec2 rotatingPointOffset = glm::vec2(0.0f, 0.0f);
	float torque = 0.0f;
	float rotationSpeed = 0.0f;
	float drag = 0.05f;
	float angularDrag = 0.07f;

	float stepCounter = 0.0f;
	const float physDeltaTime = 1.0f / 60.0f;

	Functionality functions;

	// Collision
	glm::vec2 collidingDirection = glm::vec2(0.0f, 0.0f);
	float collidingAngle;
	float CollidingSideAngle;

	//Objects colliding with this object/collider
	std::vector<Object*>objects;
	//Point that collided (from the object that collided)
	glm::vec2 point;
	//Normal of the collision
	glm::vec2 normal;

	//Private collision functions
	bool PointCheck(glm::vec2 p, std::vector<Side>sides);
	//Calculate collision normal
	void CalculateCollisionNormal(std::vector<Side>sides);

	//Find colliding object
	bool FindCollidingObject(Object* obj);
	//Remove colliding object
	void RemoveCollidingObject(Object* obj);

	//Collision event functions
	void OnCollisionEnter(Object* obj);
	void OnCollisionExit();
};