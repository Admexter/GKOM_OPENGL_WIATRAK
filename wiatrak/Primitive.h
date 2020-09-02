#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Object.h"
#include "ShaderProgram.h"

using namespace std;

class Primitive :
	public Object
{
	virtual void prepareVertices() = 0;
	virtual void prepareIndices() = 0;
public:
	GLuint VBO, EBO, VAO;
	ShaderProgram* theProgram;
	GLuint texture0;

	glm::vec3 coordinates;
	glm::vec3 rotations;
	glm::vec3 size;
	glm::vec3 colour;

	vector<GLfloat> vertices;
	vector<GLuint> indices;

	Primitive(glm::vec3 coordinates, glm::vec3 rotations, int texture, ShaderProgram* shader, glm::vec3 size = { 1.f, 1.f, 1.f })
		: coordinates(coordinates), rotations(rotations), size(size), texture0(texture), theProgram(shader)
	{}

	void init() {
		prepareVertices();
		prepareIndices();
		prepareVAO();
	}

	void prepareVAO() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		// vertex geometry data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// vertex color data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// vertex texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	}

	
	void move(glm::vec3 vector) override {
		coordinates += vector;
	}

	void scale(glm::vec3 vector) override {
		size += vector;
	}

	void rotate(glm::vec3 vector) override {
		rotations += vector;
	}

	void draw() override {
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, coordinates);

		trans = glm::rotate(trans, glm::radians(rotations.x), glm::vec3(1.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(rotations.y), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(rotations.z), glm::vec3(0.0f, 0.0f, 1.0f));

		trans = glm::scale(trans, size);

		theProgram->Use();
		theProgram->setMat4("transform", trans);

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glUniform1i(glGetUniformLocation(theProgram->get_programID(), "Texture0"), 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

};
