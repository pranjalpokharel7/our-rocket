// -*-c++-*- 
// Simple camera class
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Maths/vec.hpp"
#include "../Maths/Matrix.hpp"

#include <iostream>

// Nothing much to implement here

class Camera
{
  // It tracks the camera's location
  FMath::Vec3<float> camera_front; 
  FMath::Vec3<float> camera_pos;
  FMath::Vec3<float> world_up;
public :
  Camera() : camera_front{-1.0f,0.0f,0.0f}, camera_pos{}, world_up{-1.0f,0.0f,0.0f} {}
  
  FMath::Mat4<float> ViewMatrix()
  {
    auto view = FMath::lookAtMatrix(camera_pos,camera_pos + camera_front, {0.0f,1.0f,0.0f});
    return view;
  }

  // returns the Euler angles for the current camera orientation
  FMath::Vec3<float> CameraOrientation(); // To be implemented
};

#endif 
