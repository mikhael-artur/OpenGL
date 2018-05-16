#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"

// Represents a single particle and its state
struct Particle {
  glm::vec3 position, velocity;
  glm::vec4 color;
  float size;
  float life; // Remaining life of the particle. if < 0 : dead and unused.
  float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

  Particle() : position(0.0f), velocity(0.0f), color(1.0f), size(0.0f), life(0.0f), cameradistance(-1.0f) { }

  bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator {
public:
  // Constructor
  ParticleGenerator(const Shader& shader, const char texture_path[], const string &texture_directory, unsigned int amount);

  // Update all particles
  virtual void update(float dt, unsigned int new_particles, glm::vec3 camera_position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;

  // Render all particles
  void render();

  void sortParticles();

protected:
  // Initializes buffer and vertex attributes
  void init();

  // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
  unsigned int firstUnusedParticle();

  // Respawns particle
  virtual void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;

  // State
  std::vector<Particle> particles_;
  std::vector<float> g_particle_position_size_data_;
  std::vector<unsigned char> g_particle_color_data_;
  unsigned int amount_;
  unsigned int particles_count_;

  // Render state
  Shader shader_;
  Texture texture_;
  unsigned int VAO_;
  unsigned int billboard_vertex_buffer_;
  unsigned int particles_position_buffer_;
  unsigned int particles_color_buffer_;

  // Stores the index of the last particle used (for quick access to next dead particle)
  unsigned int last_used_particle_;


};

#endif
