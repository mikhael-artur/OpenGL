#ifndef SMOKE_GENERATOR_H
#define SMOKE_GENERATOR_H
#include "particle_generator.h"

class SmokeGenerator : public ParticleGenerator {
public:
	// Constructor
	SmokeGenerator(Shader shader, GLuint amount, const char texture_path[]);

	// Update all particles
	void Update(GLfloat dt, GLuint newParticles, glm::vec3 camera_position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

protected:
	// Respawns particle
	void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

};

#endif
