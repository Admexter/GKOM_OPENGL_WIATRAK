#pragma once
#pragma once
#include "Object.h"
#include "PointLight.h"
#include "Cube.h"
class BlockPointLight :
	public PointLight,
	public Object {
public:
	Primitive* block;

	BlockPointLight(ShaderProgram* _shader, glm::vec3 _lightPos,
		glm::vec3 _diffuse = { 0.5f, 0.5f, 0.5f },
		glm::vec3 _ambient = { 0.2f, 0.2f, 0.2f }, glm::vec3 _specular = { 1.0f, 1.0f, 1.0f },
		float _constant = 1.0f, float _linear = 0.09f, float _quadratic = 0.032f) :
		PointLight(_shader, _lightPos, _ambient, _diffuse, _specular, _constant, _linear, _quadratic) {
	}

	~BlockPointLight() {
		delete block;
	}

	void draw() {
		block->theProgram->Use();
		block->theProgram->setVec3("colour", diffuse);
		block->draw();
	}

	void move(glm::vec3 vector) {
		PointLight::move(vector);
		block->move(vector);
	}

	void scale(glm::vec3 vector) {
		block->scale(vector);
	}

	void rotate(glm::vec3 vector) {
		block->rotate(vector);
	}

	void changeColour(glm::vec3 vector) {
		PointLight::changeColour(vector);
	}

};