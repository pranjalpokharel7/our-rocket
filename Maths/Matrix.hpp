
#ifndef MATRIX4_HPP
#define MATRIX4_HPP
// Single header simple matrix library like vector
#include "vec.hpp"
#include <type_traits>
#include <cmath>
// We will only be implementing 4x4 matrix for now .. and that's all we need

namespace FMath
{
  template <typename T, typename = std::enable_if_t<std::is_integral_v<T> ||
						    std::is_floating_point_v<T>>>
  class Mat4
  {
    // Opengl uses column major order .. so we will go with that .. Might look opposite but its what OpengL use internally
    T elements[4][4] = {};
  public:
    Mat4() = default;
    
    // Pass one to initialize as identifty array 
    Mat4(T num)
    {
      for (int i = 0; i < 4; ++i)
	elements[i][i] = num;
    }
    
    T* operator[](const unsigned i)
    {
      return &elements[i][0];
    }
    
    constexpr Mat4 translate(const Vec3<T>& vec)
    {
      Mat4 id{1.0f};
      id[3][0] = vec.x;
      id[3][1] = vec.y;
      id[3][2] = vec.z;
      return id * *this;
    }

    constexpr Mat4 rotateX(float angle)
    {
      Mat4 id{1.0f};
      id[1][1] = std::cos(angle);
      id[1][2] = std::sin(angle);
      id[2][1] = -id[1][2];
      id[2][2] = id[1][1];
      return id * *this;
    }

    constexpr Mat4 rotateY(float angle)
    {
      Mat4 id{1.0f};
      id[0][0] = std::cos(angle);
      id[0][2] = -std::sin(angle);
      id[2][0] = id[0][2];
      id[2][2] = id[0][0];
      return id * *this;
    }

    constexpr Mat4 rotateZ(float angle)
    {
      Mat4 id{1.0f};
      id[0][0] = std::cos(angle);
      id[0][1] = std::sin(angle);
      id[1][0] = -id[0][1];
      id[1][1] = id[0][0];
      return id * *this; 
    }

    // Generic rotation function .. every angle here are in radians 
    constexpr Mat4* rotate(const Vec3<T>& vec, float angle)
    {
      Mat4 id{1.0f};
      auto axis = vec.unitVec();
      const float sin = std::sin(angle), cos = std::cos(angle);
      id[0][0] = cos + axis.x * axis.x * (1-cos);
      id[1][1] = cos + axis.y * axis.y * (1-cos);
      id[2][2] = cos + axis.z * axis.z * (1-cos);

      id[1][0] = axis.x * axis.y * (1-cos) - axis.z;
      id[2][0] = axis.x * axis.z * (1-cos) + axis.y;

      id[0][1] = axis.y * axis.x * (1-cos) + axis.z * sin;
      id[2][1] = axis.y * axis.z * (1-cos) - axis.x * sin;

      id[0][2] = axis.z * axis.x * (1-cos) - axis.y * sin;
      id[1][2] = axis.z * axis.y * (1-cos) + axis.x * sin;

      return id * *this;
	       
    }

    constexpr Mat4* operator*(const Mat4& mat)
    {
      Mat4 result = Mat4(T{});
      for (int i = 0; i < 4; ++i)
      {
	for (int k = 0; k < 4; ++k)
	{
	  for (int j = 0; j < 4; ++j)
	    result[i][j] += elements[i][k] * mat[k][j];
	}
	
      }
      result;
    }

    Mat4 perspective(float aspect_ratio, float fovy = 45.0f, float near_plane = 0.1f, float far_plane = 10.0f)
    {
      Mat4 id{1.0f}; // Need c++17 conformance compiler
      float tan_fovy = std::sin(fovy)/std::cos(fovy);
      float top = near_plane * tan_fovy;
      float right = aspect_ratio * top;
      
      id[0][0] = near_plane / right;
      id[1][1] = near_plane / top;
      id[2][3] = -1;
      id[3][2] = - 2 * far_plane * near_plane / (far_plane - near_plane);
      id[2][2] = -(far_plane+near_plane) / (far_plane - near_plane);
      id[3][3] = 0;
      return id * *this;
    }
    
    T* value_ptr()
    {
      return elements;
    }

  };

  Mat4<float> perspectiveProjection(float aspect_ratio, float fovy = 45.0f, float near_plane = 0.1f, float far_plane = 10.0f)
  {
    Mat4 id{1.0f}; // Need c++17 conformance compiler
    float tan_fovy = std::sin(fovy)/std::cos(fovy);
    float top = near_plane * tan_fovy;
    float right = aspect_ratio * top;

    id[0][0] = near_plane / right;
    id[1][1] = near_plane / top;
    id[2][3] = -1;
    id[3][2] = - 2 * far_plane * near_plane / (far_plane - near_plane);
    id[2][2] = -(far_plane+near_plane) / (far_plane - near_plane);
    id[3][3] = 0;
    return id;
  }

  template <typename U>
  std::ostream& operator<<(std::ostream& os, Mat4<U> matrix)
  {
    std::cout << "\nOutput in Row major order -> \n";
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
	os << '[' << i << ']' << '[' << j << ']' <<" -> " << matrix[j][i] << "   "; 
      }
      os << '\n';
    }
    return os << std::endl;
  }

  
  Mat4<float> lookAtMatrix(const Vec3<float>& cameraPos, const Vec3<float> &target, const Vec3<float>& up)
  {
    auto forward_vec = (cameraPos - target).unitVec();
    auto right = FMath::Vec3<float>::cross(up,forward_vec);

    auto now_up = FMath::Vec3<float>::cross(forward_vec,right).unitVec();
    Mat4 id{1.0f};
    id[0][0] = right.x;
    id[1][0] = right.y;
    id[2][0] = right.z;
    id[3][0] = -cameraPos.dot(right);

    id[0][1] = now_up.x;
    id[1][1] = now_up.y;
    id[2][1] = now_up.z;

    id[3][1] = -cameraPos.dot(now_up);

    id[0][2] = forward_vec.x;
    id[1][2] = forward_vec.y;
    id[2][2] = forward_vec.z;
    id[3][2] = -cameraPos.dot(forward_vec);
    return id; 
  }
      
      
}

#endif 
