#include "particle_generator.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <algorithm>

// Constructor
ParticleGenerator::ParticleGenerator(Shader shader, GLuint amount, const char texture_path[]) : shader(shader), amount(amount) {
    texture.id = loadTexture(texture_path);
    last_used_particle = 0;
    init();
}

// Render all particles
void ParticleGenerator::Draw() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(GLfloat) * 4, &particles_position[0]);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(GLubyte) * 4, &particles_color[0]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles_count);
	glBindVertexArray(0);
}

void ParticleGenerator::sortParticles() {
	std::sort(particles.begin(), particles.end());
}

// Initializes buffer and vertex attributes
void ParticleGenerator::init() {
	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, amount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, amount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (unsigned int i = 0; i < amount; i++) {
		particles.push_back(Particle());
	}

	particles_position.resize(4 * amount);
	particles_color.resize(4 * amount);
}

// Returns the first Particle index that's currently unused e.g. life <= 0.0f or 0 if no particle is currently inactive
unsigned int ParticleGenerator::firstUnusedParticle() {
	// First search from last used particle, this will usually return almost instantly
	for (unsigned int i = last_used_particle; i < amount; i++) {
		if (particles[i].Life <= 0.0f) {
			last_used_particle = i;
			return last_used_particle;
		}
	}

	// Otherwise, do a linear search
	for (unsigned int i = 0; i < last_used_particle; i++) {
		if (particles[i].Life <= 0.0f) {
			last_used_particle = i;
			return last_used_particle;
		}
	}
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    last_used_particle = 0;
    return last_used_particle;
}

