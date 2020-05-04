#include <Object.h>

Object::Object() {}
Object::~Object() {}

void Object::ApplyGravity() {
	velocity.y -= 0.5 * mass * gravity * physDeltaTime;
}

void Object::AddForce(glm::vec2 direction, float force) {
	glm::normalize(direction);
	velocity += direction * force / mass * physDeltaTime;
}

void Object::AddForce(float angle, float force) {
	// decimals to radians
	angle *= (4 * atan(1)) / 180;
	glm::vec2 direction(cos(angle), sin(angle));
	AddForce(direction, force);
}

void Object::AddTorque(float force) {
	torque -= force / 10.0f / mass * physDeltaTime;
}

void Object::Impulse(glm::vec2 direction, float force) {}

void Object::Impulse(float angle, float force) {}

void Object::SetVelocity(glm::vec2 _velocity) {
	velocity = _velocity;
}
void Object::SetVelocity(float x, float y) {
	SetVelocity(glm::vec2(x, y));
}

void Object::SetRotation(float _rotation) {
	// decimals to radians
	rotation = _rotation * (4 * atan(1)) / 180;
}

void Object::SetRotationPoint(glm::vec2 offset) {
	rotatingPointOffset = offset;
}

void Object::SetRotationPoint(float x, float y) {
	rotatingPointOffset = glm::vec2(x, y);
}

void Object::SetDrag(float value) {
	drag = value;
}

void Object::SetAngularDrag(float value) {
	angularDrag = value;
}

void Object::SetGravity(float _gravity) {
	gravity = _gravity;
}

void Object::applyDrag() {
	velocity *= (1 - drag) / mass;
}

void Object::applyAngularDrag() {
	torque *= (1 - angularDrag);
}

void Object::ApplyPhysics() {
	stepCounter += functions.DeltaTime();
	if (stepCounter >= physDeltaTime) {
		if (useGravity) {
			ApplyGravity();
		}
		// Apply Drags
		applyAngularDrag();
		applyDrag();

		// Move and rotate Object
		applyMovement();
	}
}

void Object::applyMovement() {
	// Adding temp velocity/rotation continuing force calculations in backgorund
	// from where left after freezing movement/rotation
	// so if unfrozen during movement the force has still been calculated
	glm::vec2 tempVelocity = velocity;
	tempVelocity.x *= abs(FreezeMovementX - 1);
	tempVelocity.y *= abs(FreezeMovementY - 1);
	position += tempVelocity;

	// Rotation
	if(!freezeRotation) {
		float tempTorque = torque;
		rotation += tempTorque;

		// Apply rotation point offset
		position.x += (sin(-rotation) * rotatingPointOffset.x + cos(rotation) * rotatingPointOffset.y)* tempTorque;
		position.y += (sin(rotation) * rotatingPointOffset.y + cos(-rotation) * rotatingPointOffset.x) * tempTorque;
	}
}

// -------------- COLLISION ---------------------------------------

bool Object::IsColliding(Object* other) {
	//Checks needs to be done both ways
	//This is because it is possible (and easy) for the other object to have points
	//colliding with the other, without the other having any points doing the same


	//Point and sides containers
	std::vector<glm::vec2>points = GetPoints();
	std::vector<Side>sides = other->GetSides();

	//First check points of self against sides of other
	for (int i = 0; i < points.size(); i++) {
		//If any collide, return true
		if (PointCheck(points[i], sides)) {
			//Add gameobject to list of colliding if not yet there
			//Call collision enter
			if (!FindCollidingObject(other)) {
				//Only do one getcomponent call for optimization
				Object* col = other;

				objects.push_back(other);
				col->objects.push_back(this);

				OnCollisionEnter(other);
				col->OnCollisionEnter(this);
			}

			return true;
		}
	}
	//Then check points of other against sides of self
	points = other->GetPoints();
	sides = GetSides();
	for (int i = 0; i < points.size(); i++) {
		//If any collide, return true
		if (PointCheck(points[i], sides)) {
			//Add gameobject to list of colliding if not yet there
			if (!FindCollidingObject(other)) {
				//Only do one getcomponent call for optimization
				Object* col = other;

				objects.push_back(other);
				col->objects.push_back(this);

				OnCollisionEnter(other);
				col->OnCollisionEnter(this);
			}
			return true;
		}
	}
	//No collisions detected
	//Check if it is exiting collision
	//Note that objects are always added mutually to both lists, so checking only one is sufficient
	if (FindCollidingObject(other)) {
		RemoveCollidingObject(other);
		//remove from the other as well
		//Only do one getcomponent call for optimization
		Object* col = other;
		col->RemoveCollidingObject(this);
		//Call collision exit
		OnCollisionExit();
		col->OnCollisionExit();
	}
	return false;
}

// ---- PRIVATE ----

bool Object::PointCheck(glm::vec2 p, std::vector<Side>sides) {
	//Point p is inside the polygon created by the sides,
	//if for all sides S (a, b), it is true that
	//	(b - a) x (q - a) >= 0

	//Iterate sides
	for (int i = 0; i < sides.size(); i++)
	{
		//Check if cross is negative
		if (glm::cross(sides[i].b - sides[i].a, p - sides[i].a) < 0){
			//If it is, point can't be inside
			return false;
		}
	}
	//Cleared all checks
	//Is inside all the sides
	return true;
}

bool Object::FindCollidingObject(Object* obj) {
	//Check list for the object
	auto it = std::find(objects.begin(), objects.end(), obj);
	if (it == objects.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Object::RemoveCollidingObject(Object* obj) {
	//find the place of the object
	//this function is only called when it is proven that the object exists on the list
	//so no need for checks
	auto it = std::find(objects.begin(), objects.end(), obj);
	objects.erase(it);
}

//Private collision event functions
//Utilize some function pointers with these for
//user defined events?
void Object::OnCollisionEnter(Object* obj) {}
void Object::OnCollisionExit() {}

std::vector<glm::vec2> Object::GetPoints() {
	std::vector<glm::vec2>points;
	points.push_back(glm::vec2(position.x - size.x / 2, position.y - size.y / 2));
	points.push_back(glm::vec2(position.x - size.x / 2, position.y + size.y / 2));
	points.push_back(glm::vec2(position.x + size.x / 2, position.y + size.y / 2));
	points.push_back(glm::vec2(position.x + size.x / 2, position.y - size.y / 2));
	   
	std::cout << "Points:\n";
	for (std::vector<glm::vec2>::iterator it = points.begin(); it != points.end(); ++it) {
		std::cout << (*it).x << " - " << (*it).y << "\n";
	}

	return points;
}

std::vector<Side> Object::GetSides() {
	std::vector<glm::vec2>points = GetPoints();
	std::vector<Side>sides;
	int i = 0;
	for (; i < points.size() - 1; i++) {
		sides.push_back({ points[i], points[i + 1] });
	}
	//One more from the last point to the first one
	sides.push_back({ points[i], points[0] });
	return sides;
}