#pragma once
#include "ShaderProgram.h"
#include <string>
using namespace std;
class PointLight
{
public:
	static int pointLightsQuantity;
	ShaderProgram* shader;
	int number;
	glm::vec3 lightPos;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	PointLight(ShaderProgram * _shader,  glm::vec3 _lightPos, 
		glm::vec3 _ambient = { 0.2f, 0.2f, 0.2f }, glm::vec3 _diffuse = { 0.5f, 0.5f, 0.5f }, glm::vec3 _specular = { 1.0f, 1.0f, 1.0f },
		float _constant = 1.0f, float _linear = 0.09f, float _quadratic = 0.032f): 
		shader(_shader), lightPos(_lightPos), 
		ambient(_ambient), diffuse(_diffuse), specular(_specular),
		constant(_constant), linear(_linear), quadratic(_quadratic)
	{
		number = pointLightsQuantity;
		string number_str = to_string(number);
		
		shader->Use();
		shader->setVec3(("pointLights[" + number_str + "].position").c_str(), lightPos);
		shader->setVec3(("pointLights[" + number_str + "].ambient").c_str(), ambient);
		shader->setVec3(("pointLights[" + number_str + "].diffuse").c_str(), diffuse);
		shader->setVec3(("pointLights[" + number_str + "].specular").c_str(), specular);
		shader->setFloat(("pointLights[" + number_str + "].constant").c_str(), constant);
		shader->setFloat(("pointLights[" + number_str + "].linear").c_str(), linear);
		shader->setFloat(("pointLights[" + number_str + "].quadratic").c_str(), quadratic);
		
		pointLightsQuantity++;
		shader->setInt("pointLightsQuantity", pointLightsQuantity);
	}

	void move(glm::vec3 vector) {
		lightPos += vector;
		shader->Use();
		shader->setVec3(("pointLights[" + to_string(number) + "].position").c_str(), lightPos);
	}

	void changeColour(glm::vec3 colour) {
		diffuse = colour;
		shader->Use();
		shader->setVec3(("pointLights[" + to_string(number) + "].diffuse").c_str(), diffuse);
	}
};

