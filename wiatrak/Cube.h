#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "ShaderProgram.h"
#include "Primitive.h"

using namespace std;

class Cube : public Primitive
{
public: 
	Cube(glm::vec3 coordinates, glm::vec3 size, int texture, ShaderProgram * shader,
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
		Primitive(coordinates, rotation, texture, shader, size)
	{
		init();
	}

	void prepareVertices() override {
		GLfloat vert[] = {
			 .5f, .5f, .5f,  -.5f, .5f, .5f,  -.5f,-.5f, .5f,  .5f,-.5f, .5f, // v0,v1,v2,v3 (front)
			 .5f, .5f, .5f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,  .5f, .5f,-.5f, // v0,v3,v4,v5 (right)
			 .5f, .5f, .5f,   .5f, .5f,-.5f,  -.5f, .5f,-.5f, -.5f, .5f, .5f, // v0,v5,v6,v1 (top)
			-.5f, .5f, .5f,  -.5f, .5f,-.5f,  -.5f,-.5f,-.5f, -.5f,-.5f, .5f, // v1,v6,v7,v2 (left)
			-.5f,-.5f,-.5f,   .5f,-.5f,-.5f,   .5f,-.5f, .5f, -.5f,-.5f, .5f, // v7,v4,v3,v2 (bottom)
			 .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,  -.5f, .5f,-.5f,  .5f, .5f,-.5f  // v4,v7,v6,v5 (back)
		};

		// normal array
		GLfloat normals[] = {
			 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,  // v0,v1,v2,v3 (front)
			 1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  // v0,v3,v4,v5 (right)
			 0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,  // v0,v5,v6,v1 (top)
			-1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  // v1,v6,v7,v2 (left)
			 0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,  // v7,v4,v3,v2 (bottom)
			 0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1   // v4,v7,v6,v5 (back)
		};

		// texture coord array
		GLfloat texCoords[] = {
			1, 0,   0, 0,   0, 1,   1, 1,               // v0,v1,v2,v3 (front)
			0, 0,   0, 1,   1, 1,   1, 0,               // v0,v3,v4,v5 (right)
			1, 1,   1, 0,   0, 0,   0, 1,               // v0,v5,v6,v1 (top)
			1, 0,   0, 0,   0, 1,   1, 1,               // v1,v6,v7,v2 (left)
			0, 1,   1, 1,   1, 0,   0, 0,               // v7,v4,v3,v2 (bottom)
			0, 1,   1, 1,   1, 0,   0, 0                // v4,v7,v6,v5 (back)
		};

		size_t elements = _countof(vert) + _countof(normals) + _countof(texCoords);

		int texIndex = 0;
		for (int i = 0; i < elements; i = i + 3) {
			vertices.push_back(vert[i]);
			vertices.push_back(vert[i + 1]);
			vertices.push_back(vert[i + 2]);
			vertices.push_back(normals[i]);
			vertices.push_back(normals[i + 1]);
			vertices.push_back(normals[i + 2]);
			vertices.push_back(texCoords[texIndex]);
			vertices.push_back(texCoords[texIndex + 1]);
			texIndex += 2;

		}
	}

	void prepareIndices() override {
		// index array for glDrawElements()
		// A cube requires 36 indices = 6 sides * 2 tris * 3 verts
		GLuint ind[] = {
			 0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
			 4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
			 8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
			12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
			16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
			20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
		};

		for (int i = 0; i < 36; i++)
			indices.push_back(ind[i]);
	}


};

