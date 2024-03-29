#pragma once
#ifndef PLANE_H
#define PLANE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <learnopengl/shader_m.h>

#include "transformation.h"

#include <iostream>

static float grassVertices[] = {
	-0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.0f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.0f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.0f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.0f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.0f, -0.5f,  0.0f, 1.0f
};

class Plane {
	Shader ourShader;
	unsigned int VAO, VBO;
	unsigned int texture1;

public:
	Plane():ourShader("plane.vs", "plane.fs") {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);


		// load and create a texture 
		// -------------------------
		// texture 1
		// ---------
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char* data = stbi_load("resources/textures/grass1.jpeg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
		// -------------------------------------------------------------------------------------------
		ourShader.use();
		ourShader.setInt("texture1", 0);

	}
	~Plane() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void draw(const Transform& transform) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		ourShader.use();

		ourShader.setMat4("model", transform.model);
		ourShader.setMat4("view", transform.view);
		ourShader.setMat4("projection", transform.projection);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};


#endif // !PLANE_H
