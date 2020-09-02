#pragma once
#include "Primitive.h"
#define _USE_MATH_DEFINES
#include <math.h>
class Cylinder : public Primitive
{
public: 
	Cylinder(glm::vec3 coordinates, glm::vec3 size, int texture, ShaderProgram* shader,
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
		Primitive(coordinates, rotation, texture, shader, size) 
	{
		init();
	}


	void prepareVertices() override {
		std::vector<GLfloat>  vert;
		std::vector<GLfloat>  normals;
		std::vector<GLfloat>  texCoords;

		int slices = 50;
		GLfloat radius = 1.f;

		//central point
		vert.push_back(0.f);
		vert.push_back(0.5f);
		vert.push_back(0.f);

		for (int i = 0; i < slices; i++) {
			GLfloat theta = ((GLfloat)i) / slices * 2 * M_PI;
			vert.push_back(radius * cos(theta));
			vert.push_back(0.5f);
			vert.push_back(radius * sin(theta));
		}

		vert.push_back(0.f);
		vert.push_back(-0.5f);
		vert.push_back(0.f);

		for (int i = 0; i < slices; i++) {
			GLfloat theta = ((GLfloat)i) / slices * 2 * M_PI;
			vert.push_back(radius * cos(theta));
			vert.push_back(-0.5f);
			vert.push_back(radius * sin(theta));
		}

		//powierzchnia boczna
		for (int i = 0; i < slices; i++) {
			GLfloat theta = ((GLfloat)i) / slices * 2 * M_PI;
			GLfloat nextTheta = ((GLfloat)i + 1) / slices * 2 * M_PI;

			vert.push_back(radius * cos(theta));
			vert.push_back(0.5f);
			vert.push_back(radius * sin(theta));

			vert.push_back(radius * cos(theta));
			vert.push_back(-0.5f);
			vert.push_back(radius * sin(theta));
		}

		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);

		for (int i = 0; i < slices; i++) {
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}

		normals.push_back(0);
		normals.push_back(-1);
		normals.push_back(0);

		for (int i = 0; i < slices; i++) {
			normals.push_back(0);
			normals.push_back(-1);
			normals.push_back(0);
		}

		//powierzchnia boczna

		for (int i = 0; i < slices; i++) {
			GLfloat theta = ((GLfloat)i) / slices * 2 * M_PI;
			normals.push_back(radius * cos(theta));
			normals.push_back(0);
			normals.push_back(radius * sin(theta));

			normals.push_back(radius * cos(theta));
			normals.push_back(0);
			normals.push_back(radius * sin(theta));
		}

		texCoords.push_back(0.25f);
		texCoords.push_back(0.25f);

		for (int i = 0; i < slices; i++) {
			GLfloat theta = ((GLfloat)i) / slices * 2 * M_PI;
			texCoords.push_back(0.25f * cos(theta) + 0.25f);
			texCoords.push_back(0.25f * sin(theta) + 0.25f);
		}

		texCoords.push_back(0.75f);
		texCoords.push_back(0.25f);

		for (int i = 0; i < slices; i++) {
			GLfloat theta = ((GLfloat)i) / slices * 2 * M_PI;
			texCoords.push_back(0.25f * cos(theta) + 0.75f);
			texCoords.push_back(0.25f * sin(theta) + 0.25f);
		}

		//powierzchnia boczna

		for (int i = 0; i < slices; i++) {
			texCoords.push_back(1);
			texCoords.push_back(0.5f + ((GLfloat)i / (GLfloat)slices) * 0.5f);
			texCoords.push_back(0);
			texCoords.push_back(0.5f + ((GLfloat)i / (GLfloat)slices) * 0.5f);
		}


		size_t elements = vert.size() + normals.size() + texCoords.size();

		int texIndex = 0;
		for (int i = 0; i < vert.size(); i = i + 3) {
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
		std::vector<GLuint>  ind;

		int slices = 50;

		for (int i = 0; i < slices; i++) {
			ind.push_back(0);
			int tmp;
			if (i + 2 > slices) {
				ind.push_back(i + 1);
				ind.push_back(1);
			} else {
				ind.push_back(i + 1);
				ind.push_back(i + 2);
			}
		}

		for (int i = slices + 1; i < 2 * slices + 1; i++) {
			ind.push_back(slices + 1);
			int tmp;
			if (i + 2 > 2 * slices + 1) {
				ind.push_back(i + 1);
				ind.push_back(slices + 2);
			} else {
				ind.push_back(i + 1);
				ind.push_back(i + 2);
			}
		}

		for (int i = 2 * slices + 2; i < 4 * slices + 2; i += 2) {

			ind.push_back(i);
			ind.push_back(i + 1);

			if (i + 2 > 4 * slices + 1) {
				ind.push_back(2 * slices + 2);
			} else {
				ind.push_back(i + 2);
			}


			ind.push_back(i + 1);
			if (i + 2 > 4 * slices + 1) {
				ind.push_back(2 * slices + 2);
			} else {
				ind.push_back(i + 2);
			}

			if (i + 3 > 4 * slices + 1) {
				ind.push_back(2 * slices + 3);
			} else {
				ind.push_back(i + 3);
			}
		}

		for (int i = 0; i < (int)ind.size(); i++)
			indices.push_back(ind[i]);
	}
};

