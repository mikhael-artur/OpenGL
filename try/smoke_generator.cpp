#include "smoke_generator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

// Constructor
SmokeGenerator::SmokeGenerator(const Shader& shader, const char texture_path[], const string &texture_directory, unsigned int amount)
    : ParticleGenerator(shader, texture_path, texture_directory, amount) {
}

// Update all particles
void SmokeGenerator::update(float dt, unsigned int new_particles, glm::vec3 camera_position, glm::vec3 offset) {
  // Add new particles
  for (unsigned int i = 0; i < new_particles; i++) {
    int unused_particle = firstUnusedParticle();
    respawnParticle(particles_[unused_particle], offset);
  }

  // Update all particles
  particles_count_ = 0;
  for (unsigned int i = 0; i < amount_; i++) {
    Particle &p = particles_[i];
    if (p.life > 0.0f) {
      p.life -= dt; // reduce life
      p.color.a -= (dt * 100);
      if (p.life > 0.0f) {	// particle is alive, thus update

        // move particle
        p.position += p.velocity * dt;

        // wind effect
        p.position.x += -(rand() % 3)/8.0f * dt;
        p.position.y += -(rand() % 5 - 2)/16.0f * dt;
        p.position.z += -(rand() % 7 - 2)/8.0f * dt;

        p.cameradistance = glm::length2(p.position - camera_position);

        // Fill the GPU buffer
        g_particle_position_size_data_[4*particles_count_+0] = p.position.x;
        g_particle_position_size_data_[4*particles_count_+1] = p.position.y;
        g_particle_position_size_data_[4*particles_count_+2] = p.position.z;

        g_particle_position_size_data_[4*particles_count_+3] = p.size;

        g_particle_color_data_[4*particles_count_+0] = p.color.r;
        g_particle_color_data_[4*particles_count_+1] = p.color.g;
        g_particle_color_data_[4*particles_count_+2] = p.color.b;
        g_particle_color_data_[4*particles_count_+3] = p.color.a;
      } else {
        // Particles that just died will be put at the end of the buffer in SortParticles();
  			p.cameradistance = -1.0f;
      }
      particles_count_++;
    }
  }

  sortParticles();
}

// Respawns particle
void SmokeGenerator::respawnParticle(Particle &particle, glm::vec3 offset) {
  particle.life = 1.0f; // This particle will live 1 second.
	particle.position = glm::vec3(-0.244f, -0.065f, 2.48f);
	particle.velocity = glm::vec3(0.0f, 0.02f, 0.0f);

  int color = 200 + rand() % 56;
	particle.color.r = color;
	particle.color.g = color;
	particle.color.b = color;
	particle.color.a = 255;
	particle.size =  0.025f;
}
