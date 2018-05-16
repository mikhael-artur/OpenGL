#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <glad/glad.h>
#include <vector>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include "stb_image.h"
#include "mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <algorithm>

// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
	GLfloat Size;
    GLfloat Life; // Remaining life of the particle. if < 0 : dead and unused.
	GLfloat CameraDistance; // *Squared* distance to the camera. if dead : -1.0f

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Size(0.0f), Life(0.0f), CameraDistance(-1.0f) { }
	
	bool operator<(Particle& that){
        // Sort in reverse order : far particles drawn first.
        return this->CameraDistance > that.CameraDistance;
    }
};

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path);

// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, GLuint amount, const char texture_path[]);
    // Update all particles
    virtual void Update(GLfloat dt, GLuint newParticles, glm::vec3 camera_position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;
    // Render all particles
    void Draw();
	void sortParticles();
protected:
    // State
    std::vector<Particle> particles;
    std::vector<GLfloat> particles_position;
    std::vector<unsigned char> particles_color;
    GLuint amount;
	GLuint particles_count;
	unsigned int last_used_particle;
    // Render state
    Shader shader;
    Texture texture;
    GLuint VAO1;
    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    virtual void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;
};


#endif