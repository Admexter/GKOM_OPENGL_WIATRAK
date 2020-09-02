#pragma once
#include "BlockPointLight.h"
#include "Textures.h"

class CubicPointLight : 
	public BlockPointLight {
public:
	CubicPointLight(ShaderProgram* _shader, Textures* texs, glm::vec3 _lightPos,
		ShaderProgram* _blockShader, glm::vec3 _diffuse = { 0.5f, 0.5f, 0.5f }, glm::vec3 _blockSize = { 0.25f, 0.25f, 0.25f },
		glm::vec3 _ambient = { 0.2f, 0.2f, 0.2f }, glm::vec3 _specular = { 1.0f, 1.0f, 1.0f },
		float _constant = 1.0f, float _linear = 0.09f, float _quadratic = 0.032f) :
		BlockPointLight(_shader, _lightPos, _diffuse, _ambient, _specular, _constant, _linear, _quadratic) {
		block = new Cube(_lightPos, _blockSize, texs->get("textures/wood2.jpg"), _blockShader);
	}
};