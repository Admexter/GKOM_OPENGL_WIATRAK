#pragma once
#include "Composite.h"
#include "Cube.h"
#include "Textures.h"
#include "Frustum.h"
#include "Sphere.h"
#include "Cylinder.h"

class Windmill: public Composite {
	glm::vec3 position;
	Cube* body;
	Cube* body1;
	Cube* column1;
	Cube* column2;
	Cube* column3;
	Cube* column4;
	Cube* body2;
	Frustum* body3;
	Cube* body4;
	Frustum* body5;
	Cube* shaft;
	Cube* fan[4];
	Cube* blades[4];

	const float n = 0.5;
	const glm::vec3 wheel_center = glm::vec3(0, 10 * n, 1.9 * n);
	const glm::vec3 wheel_ellipse = glm::vec3(4.0 * n, 4.0 * n * glm::cos(glm::pi<float>() / 9), 4.0 * n * glm::sin(glm::pi<float>() / 9));
	
public:
	Windmill(glm::vec3 position, ShaderProgram* shader, Textures* texs): position(position) {
		body = new Cube(position + glm::vec3(0, 0, 0), glm::vec3(5.2 * n, 0.5 * n, 5.2 * n), texs->get("textures/wood2.jpg"), shader);
		items.push_back(body);
		body1 = new Cube(position + glm::vec3(0, 1.25 * n, 0), glm::vec3(5 * n, 2.5 * n, 5 * n), texs->get("textures/wood1.jpg"), shader);
		items.push_back(body1);
		column1 = new Cube(position + glm::vec3(2.8 * n, 2.2 * n, 0), glm::vec3(0.5 * n, 1.5 * n, 0.5 * n), texs->get("textures/wood2.jpg"), shader, glm::vec3(0, 0, -45.0));
		items.push_back(column1);
		column2 = new Cube(position + glm::vec3(-2.8 * n, 2.2 * n, 0), glm::vec3(0.5 * n, 1.5 * n, 0.5 * n), texs->get("textures/wood2.jpg"), shader, glm::vec3(0, 0, 45.0));
		items.push_back(column2);
		column3 = new Cube(position + glm::vec3(0, 2.2 * n, 2.8 * n), glm::vec3(0.5 * n, 1.5 * n, 0.5 * n), texs->get("textures/wood2.jpg"), shader, glm::vec3(45.0, 0, 0));
		items.push_back(column3);
		column4 = new Cube(position + glm::vec3(0, 2.2 * n, -2.8 * n), glm::vec3(0.5 * n, 1.5 * n, 0.5 * n), texs->get("textures/wood2.jpg"), shader, glm::vec3(-45.0, 0, 0));
		items.push_back(column4);
		body2 = new Cube(position + glm::vec3(0, 2.75 * n, 0), glm::vec3(7.2 * n, 0.5 * n, 7.2 * n), texs->get("textures/wood2.jpg"), shader);
		items.push_back(body2);
		body3 = new Frustum(position + glm::vec3(0, 6.5 * n, 0), glm::vec3(5 * n, 7.2 * n, 5 * n), texs->get("textures/wood1.jpg"), shader);
		items.push_back(body3);
		body4 = new Cube(position + glm::vec3(0, 8.55 * n, -0.2 * n), glm::vec3(3.5 * n, 0.5 * n, 3.9 * n), texs->get("textures/wood2.jpg"), shader);
		items.push_back(body4);
		body5 = new Frustum(position + glm::vec3(0, 10.0 * n, -0.2 * n), glm::vec3(3.2 * n, 2.5 * n, 3.6 * n), texs->get("textures/wood1.jpg"), shader);
		items.push_back(body5);

		shaft = new Cube(position + glm::vec3(0, 10.0 * n, 1.9 * n), glm::vec3(0.6 * n, 1.5 * n, 0.6 * n), texs->get("textures/wood2.jpg"), shader, glm::vec3(70.0, 0, 0));
		items.push_back(shaft);

		glm::vec3 offsets[4] = { {0, 1, -1}, {1, 0, 0}, {0, -1, 1}, {-1, 0, 0} };

		for (int i = 0; i < 4; i++) {
			fan[i] = new Cube(position + wheel_center + offsets[i] * wheel_ellipse, glm::vec3(0.3 * n, 8.0 * n, 0.3 * n), texs->get("textures/wood1.jpg"), shader, glm::vec3(-20.0, 0, 90 * i));
			items.push_back(fan[i]);
			blades[i] = new Cube(position + wheel_center + offsets[i] * wheel_ellipse * glm::vec3(1.2), glm::vec3(2.4 * n, 6.0 * n, 0.2 * n), texs->get("textures/wood1.jpg"), shader, glm::vec3(-20.0, 0.0, 90.0 * i));
			items.push_back(blades[i]);
		}
	}

	// r should be in range 0.0 to 1.0
	void set_fan_rot(float r) {
		const float TAU = glm::two_pi<float>();
		r = glm::clamp(r, 0.0f, 1.0f);

		shaft->rotations.y = -r * 360;
		for (int i = 0; i < 4; i++) {
			auto offset = glm::vec3(glm::sin(TAU * fmod(r + (0.25 * i), 1.0)), glm::cos(TAU * fmod(r + (0.25 * i), 1.0)), -glm::cos(TAU * fmod(r + (0.25 * i), 1.0)));
			auto rotation = fmod(-r * 360.0 + (90.0 * i), 360.0);

			fan[i]->coordinates = position + wheel_center + wheel_ellipse * offset;
			fan[i]->rotations.z = rotation;
			blades[i]->coordinates = position + wheel_center + wheel_ellipse * offset * glm::vec3(1.2);
			blades[i]->rotations.z = rotation;
		}
	}
};