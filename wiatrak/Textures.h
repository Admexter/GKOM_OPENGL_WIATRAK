#pragma once
#include <GL/glew.h>
#include <SOIL.h>

#include <unordered_map>

using namespace std;

class Textures {
	unordered_map<string, int> map;
public:
	int get(string path) {
		auto r = map.find(path);
		if (r != map.end())
			return r->second;
		else {
			int r = prepareTexture(path.c_str());
			map.insert({ path, r });
			return r;
		}
	}

private:
	int prepareTexture(const char* fileName) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		return LoadMipmapTexture(GL_TEXTURE0, fileName);
	}

	GLuint LoadMipmapTexture(GLuint texId, const char* fname) {
		int width, height;
		unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
		// cout << "Loading " << fname << " texture" << endl;
		if (image == nullptr)
			throw exception("Failed to load texture file");

		GLuint texture;
		glGenTextures(1, &texture);

		glActiveTexture(texId);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}
};
