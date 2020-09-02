#pragma once
#include <vector>

#include "Object.h"
#include "Primitive.h"

using namespace std;

class Composite :
	public Object
{
public:
	vector<Object*> items;

	~Composite() {
		for (Object* item : items) {
			delete item;
		}
	}

	void move(glm::vec3 vector) override {
		for (Object* item : items) {
			item->move(vector);
		}
	}

	void scale(glm::vec3 vector) override {
		for (Object* item : items) {
			item->scale(vector);
		}
	}

	void rotate(glm::vec3 vector) override {
		for (Object* item : items) {
			item->rotate(vector);
		}
	}

	void draw() override {
		for (Object* item : items) {
			item->draw();
		}
	}

};
