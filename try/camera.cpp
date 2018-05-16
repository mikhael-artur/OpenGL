#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch) {
  movement_speed_ = SPEED;
  mouse_sensitivity_ = SENSITIVITY;
  fov_ = FOV;
  position_ = position;
  front_ = front;
  world_up_ = up;
  yaw_ = yaw;
  pitch_ = pitch;
  updateCameraVectors();
}

// Processes input received from any keyboard-like input system.
// Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(CameraMovement direction, float delta_time) {
  float velocity = movement_speed_ * delta_time;
  if (direction == FORWARD) {
    position_ += front_ * velocity;
  } else if (direction == BACKWARD) {
    position_ -= front_ * velocity;
  } else if (direction == LEFT) {
    position_ -= right_ * velocity;
  } else if (direction == RIGHT) {
    position_ += right_ * velocity;
  }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrain_pitch) {
  xoffset *= mouse_sensitivity_;
  yoffset *= mouse_sensitivity_;

  yaw_ += xoffset;
  pitch_ += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrain_pitch) {
    if (pitch_ > 89.0f) {
      pitch_ = 89.0f;
    } else if (pitch_ < -89.0f) {
      pitch_ = -89.0f;
    }
  }

  // Update front, right and up Vectors using the updated Euler angles
  updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yoffset) {
  if (fov_ >= 1.0f && fov_ <= 45.0f) {
    fov_ -= yoffset;
  } else if (fov_ <= 1.0f) {
    fov_ = 1.0f;
  } else if (fov_ >= 45.0f) {
    fov_ = 45.0f;
  }
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position_, position_ + front_, up_);
}

// Returns the camera fov
float Camera::getFov() {
  return fov_;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front);

  // Also re-calculate the Right and Up vector
  right_ = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  up_ = glm::normalize(glm::cross(right_, front_));
}

glm::vec3 Camera::getPosition() {
  return position_;
}