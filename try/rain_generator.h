#ifndef RAIN_GENERATOR_H
#define RAIN_GENERATOR_H
#include "particle_generator.h"

class RainGenerator : public ParticleGenerator {
public:
  // Constructor
  RainGenerator(const Shader& shader, const char texture_path[], const string &texture_directory, unsigned int amount);

  // Update all particles
  void update(float dt, unsigned int new_particles, glm::vec3 camera_position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

protected:
  // Respawns particle
  void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

};

#endif
