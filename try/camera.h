#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV         =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

  // Processes input received from any keyboard-like input system.
  // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void processKeyboard(CameraMovement direction, float delta_time);

  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void processMouseMovement(float xoffset, float yoffset, bool constrain_pitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void processMouseScroll(float yoffset);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 getViewMatrix();

  glm::vec3 getPosition();

  // Returns the camera fov
  float getFov();

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    // Camera Attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    // Euler Angles
    float yaw_;
    float pitch_;

    // Camera options
    float movement_speed_;
    float mouse_sensitivity_;
    float fov_;
};

#endif
