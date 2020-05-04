#include <Object.h>

Object::Object() {}
Object::~Object() {}

void Object::ApplyGravity() {
	if (useGravity) {
		AddForce(glm::vec2(0.0f, -1.0f), gravity);
	}
}

void Object::AddForce(glm::vec2 direction, float amount) {
	glm::vec2 normalizedDir = glm::normalize(direction);
	acceleration += normalizedDir * amount / mass * physDeltaTime;
}

void Object::AddForce(float angle, float amount) {
	// decimals to radians
	angle = glm::radians(angle);
	glm::vec2 direction(cos(angle), -sin(angle));
	AddForce(direction, amount);
}

void Object::AddTorque(float force) {
	torque += force / mass * physDeltaTime;
}

void Object::SetTorque(float _torque) {
	torque = _torque;
}

void Object::Impulse(glm::vec2 direction, float amount) {
	glm::vec2 normalizedDir = glm::normalize(direction);
	velocity += normalizedDir * amount / mass;
}

void Object::Impulse(float angle, float amount) {
	// decimals to radians
	angle = glm::radians(angle);
	glm::vec2 direction(cos(angle), -sin(angle));
	Impulse(direction, amount);
}

void Object::SetVelocity(glm::vec2 _velocity) {
	velocity = _velocity;
}
void Object::SetVelocity(float x, float y) {
	SetVelocity(glm::vec2(x, y));
}

void Object::SetAcceleration(glm::vec2 _acceleration) {
	acceleration = _acceleration;
}

void Object::SetAcceleration(float accelerationX, float accelerationY) {
	acceleration = glm::vec2(accelerationX, accelerationY);
}

void Object::AddRotationSpeed(float speed) {
	rotationSpeed += speed / mass;
}

void Object::SetRotationSpeed(float _rotationSpeed) {
	rotationSpeed = _rotationSpeed;
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

void Object::ApplyDrag() {
	velocity *= 1 - drag * physDeltaTime;
}

void Object::ApplyAngularDrag() {
	rotationSpeed *= 1 - angularDrag * physDeltaTime;
}

void Object::ApplyPhysics() {
	stepCounter += functions.DeltaTime();
	if (stepCounter >= physDeltaTime) {
		ApplyGravity();
		// Apply Drags
		ApplyAngularDrag();
		ApplyDrag();

		// Move and rotate Object
		ApplyMovement();
	}
}

void Object::ApplyMovement() {
	// Movement
	velocity += acceleration* physDeltaTime;
	velocity.x *= !FreezeMovementX;
	velocity.y *= !FreezeMovementY;
	position += velocity * physDeltaTime;

	// Rotation
	rotationSpeed += torque * !freezeRotation * physDeltaTime;
	rotation += rotationSpeed * physDeltaTime;

	// Apply rotation point offset
	position.x += (sin(rotation) * rotatingPointOffset.x + cos(-rotation) * rotatingPointOffset.y) * rotationSpeed * !freezeRotation;
	position.y += (sin(-rotation) * rotatingPointOffset.y + cos(rotation) * rotatingPointOffset.x) * rotationSpeed * !freezeRotation;
}



// ----------- Collider -------------------------------

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
			// Angle that the objects are realtive to each other
			// For RigidBody collision physics. Need to do in two places
			collidingDirection = glm::normalize(glm::vec2(other->position - position));
			collidingAngle = glm::degrees(atan(collidingDirection.y / collidingDirection.x));
			// The rotation of the side being collided with. Used in RigidBody rotation
			CollidingSideAngle = glm::degrees((sides[i].b - sides[i].a).x / (sides[i].b - sides[i].a).y);

			//Add gameobject to list of colliding if not yet there
			//Call collision enter
			if (!FindCollidingObject(other)) {

				//Save the collision point
				point = points[i];
				other->point = points[i];

				//Calculate collision normal
				//Self as base
				CalculateCollisionNormal(sides);

				objects.push_back(other);
				other->objects.push_back(this);

				OnCollisionEnter(other);
				other->OnCollisionEnter(this);
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
			// Angle that the objects are realtive to each other
			// For RigidBody collision physics. Need to do in two places
			collidingDirection = glm::vec2(position - abs(other->position));
			collidingAngle = glm::degrees(atan(collidingDirection.y / collidingDirection.x));
			// The rotation of the side being collided with. Used in RigidBody rotation
			CollidingSideAngle = glm::degrees((sides[i].b - sides[i].a).x / (sides[i].b - sides[i].a).y);

			//Add gameobject to list of colliding if not yet there
			if (!FindCollidingObject(other)) {

				//Save the collision point
				point = points[i];
				other->point = points[i];

				//Calculate collision normal
				//Other as base
				other->CalculateCollisionNormal(sides);

				objects.push_back(other);
				other->objects.push_back(this);

				OnCollisionEnter(other);
				other->OnCollisionEnter(this);
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
		RemoveCollidingObject(this);
		//Call collision exit
		OnCollisionExit();
		other->OnCollisionExit();
	}
	return false;
}

std::vector<glm::vec2> Object::GetPoints() {
	std::vector<glm::vec2>points;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
	for (int i = 0; i < vertices.size(); i += 3) {
		points.push_back(glm::vec2(model * glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f)));
	}
	points.erase(points.begin() + 3);
	points.erase(points.begin() + 4);

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

///////////////////////////////////////////
//Private collision functions
bool Object::PointCheck(glm::vec2 p, std::vector<Side>sides) {
	//Point p is inside the polygon created by the sides,
	//if for all sides S (a, b), it is true that
	//	(b - a) x (q - a) >= 0

	//Iterate sides
	for (int i = 0; i < sides.size(); i++) {
		//Check if cross is negative
		if (glm::cross(sides[i].b - sides[i].a, p - sides[i].a) < 0) {
			//If it is, point can't be inside
			return false;
		}
	}
	//Cleared all checks
	//Is inside all the sides
	return true;
}

void Object::CalculateCollisionNormal(std::vector<Side>sides) {
	for (int i = 0; i < sides.size(); i++) {
		glm::vec2 t = glm::inverse(glm::mat2(point - position, sides[i].b - sides[i].a))*(point - sides[i].a);
		//Check for intersections
		if (t.x <= 1.0f && t.x >= 0.0f) {
			if (t.y <= 1.0f && t.y >= 0.0f) {
				glm::vec2 side = sides[i].b - sides[i].a;
				normal = glm::rotate(side, glm::radians(-90.0f)); //-90 or 90?? or does it even matter towards which object it points
				//can return after setting the normal,
				//there shouldn't really be more than one side colliding in either case
				return;
			}
		}
	}
}

bool Object::FindCollidingObject(Object* obj) {
	//Check list for the object
	auto it = std::find(objects.begin(), objects.end(), obj);
	if (it == objects.end()) {
		return false;
	}
	else {
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