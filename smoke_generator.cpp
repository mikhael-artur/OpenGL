#include "smoke_generator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

SmokeGenerator::SmokeGenerator(Shader shader, GLuint amount, const char texture_path[]):ParticleGenerator(shader, amount, texture_path){}

void SmokeGenerator::Update(GLfloat dt, GLuint newParticles, glm::vec3 camera_position, glm::vec3 offset){
	// Add new particles
	for (unsigned int i = 0; i < newParticles; i++) {
		int unused_particle = firstUnusedParticle();
		respawnParticle(particles[unused_particle], offset);
	}

	// Update all particles
	particles_count = 0;
	for (unsigned int i = 0; i < amount; i++) {
		Particle &p = particles[i];
		if (p.Life > 0.0f) {
			p.Life -= dt; // reduce life
			p.Color.a -= (dt * 100);
			if (p.Life > 0.0f) {	// particle is alive, thus update

				// move particle
				p.Position += p.Velocity * dt;

				// wind effect
				p.Position.x += -(rand() % 3)/8.0f * dt;
				p.Position.y += -(rand() % 5 - 2)/16.0f * dt;
				p.Position.z += -(rand() % 7 - 2)/8.0f * dt;

				p.CameraDistance = glm::length2(p.Position - camera_position);

				// Fill the GPU buffer
				particles_position[4*particles_count+0] = p.Position.x;
				particles_position[4*particles_count+1] = p.Position.y;
				particles_position[4*particles_count+2] = p.Position.z;
				particles_position[4*particles_count+3] = p.Size;

				particles_color[4*particles_count+0] = p.Color.r;
				particles_color[4*particles_count+1] = p.Color.g;
				particles_color[4*particles_count+2] = p.Color.b;
				particles_color[4*particles_count+3] = p.Color.a;
			} else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.CameraDistance = -1.0f;
			}
			particles_count++;
		}
	}

	sortParticles();
}

void SmokeGenerator::respawnParticle(Particle &particle, glm::vec3 offset){
	particle.Life = 1.0f; // This particle will live 1 second.
	particle.Position = glm::vec3(4.0f, 4.0f, 4.0f);
	particle.Velocity = glm::vec3(0.0f, 2.0f, 0.0f);

	int color = 200 + rand() % 56;
	particle.Color.r = color;
	particle.Color.g = color;
	particle.Color.b = color;
	particle.Color.a = 255;
	particle.Size =  0.025f;
}
