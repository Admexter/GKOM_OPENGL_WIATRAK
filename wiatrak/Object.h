#pragma once

#include <glm/glm.hpp>

class Object
{
public:
	virtual void draw() = 0;
	virtual void move(glm::vec3 vector) = 0;
	virtual void scale(glm::vec3 vector) = 0;
	virtual void rotate(glm::vec3 vector) = 0;
};
