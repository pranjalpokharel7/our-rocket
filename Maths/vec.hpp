
// This is the math library and we will here implement 3D Math required by
// opengl. GLSL have its own math library.

// We will create a generic vector and specialize it wherever necessary
#ifndef VEC3_HPP
#define VEC3_HPP

#include <type_traits>
#include <iostream>
// Lets implement only vec3 for now
   
// Utility functions for fast calucultations .. Maynot be worth though

namespace FMath
{
  constexpr inline float pi = 3.141529f;
  constexpr inline float e = 2.71f;

  constexpr inline float DegreeToRadians(float value)
  {
    return value * pi/180.0f;
  }

  constexpr inline float RadiansToDegree(float value)
  {
    return value * 180.0f/pi;
  }
  
    constexpr float FastInverseSqrt(const float num)
    {
	const float x = num * 0.5f;
	union
	{
	    float f;
	    unsigned i;
	} conv = {num};
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= 1.5f - (x * conv.f * conv.f);
	return conv.f;
    }

    
    constexpr float NewtonSqrt(float num, float tolerance = 0.0005f)
    {
	// only works for small number .. good enough
	// Newton's method doesn't guarrantee global convergence
	auto fabs = [](auto x) { if (x >= 0) return x; return -x;};
	if (num <= 0)
	    return 0;
	float init = num / 2;
	do
	{
	    init -= (init*init-num)/(2*init);
	} while (fabs(init*init-num)>tolerance);
	return init;
    }
    constexpr float asmSqrt(float num)
    {
	// To be implemented 
	// asm("sqrt eax");
	return 0;
    }
}

namespace FMath
{
    template<typename T>
    class Vec2
    {
        public:
            union
            {
                struct { T x; T y;};
                struct { T a; T b;};
            };
        public:
            Vec2() : x(T{}), y(T{}){};
            Vec2(T X, T Y) : x(X), y(Y){};
    };

    template <typename T>
    class Vec3
    {
    public:
	union
	{
	    struct { T r; T g; T b;};
	    struct { T x; T y; T z;};
	    struct { T s; T t; T u;};
	};
    public:
	Vec3() : r(T{}),g(T{}),b(T{})
	{
	    
	}
	Vec3(T x, T y, T z) : r{x}, g{y}, b{z}
	{
	
	}
	constexpr float norm() const
	{
	    return NewtonSqrt(r * r + g * g + b * b);
	}
	constexpr Vec3 unitVec() const
	{
	    auto val = norm();
	    if (val) 
		return *this / norm();
	    return Vec3();
	}
	constexpr Vec3 operator+(const Vec3& vec) const
	{
	    return Vec3(r+vec.r,g+vec.g,b+vec.b);
	}
	constexpr Vec3 operator-(const Vec3& vec) const
	{
	    return Vec3(r-vec.r,g-vec.g,b-vec.b);
	}
	constexpr bool operator==(const Vec3& vec) const
	{
	    return r == vec.r && g == vec.g && b == vec.b;
	}
	constexpr bool operator != (const Vec3& vec) const
	{
	    return !(*this == vec);
	}
	constexpr Vec3 operator* (const Vec3& vec) const
	{
	    // do component wise multiplication .. ie dot product
	    return Vec3(r*vec.r,g*vec.g,b*vec.b);
	}
      constexpr float dot (const Vec3& vec) const
      {
	return x*vec.x + y*vec.y + z*vec.z;
      }
      static constexpr Vec3 cross(const Vec3<T> vec1, const Vec3<T>vec2) 
      {
	return Vec3(vec1.y * vec2.z - vec2.y * vec1.z,
		    vec1.z * vec2.x - vec1.x * vec2.z,
		    vec1.x * vec2.y - vec1.y * vec2.x);
      }

      // Reflection refraction bla bla could also be implement but not required for openGL .. glsl handles those things
    };

    // Implement mulitplication of vector with floating, double and integers
    // Use SFINAE here
  // Disables the template if condition not satisfied... Concepts could have also been used here but required c++20 compiler 
    template <typename T, typename U, typename = std::enable_if_t<std::is_integral_v<U> || std::is_floating_point_v<U>>>
    constexpr Vec3<T> operator*(Vec3<T> vec, U num)
    {
	return Vec3<T>(vec.r * num, vec.g * num, vec.b * num);
    }

    template <typename T, typename U, typename = std::enable_if_t<std::is_integral_v<U> || std::is_floating_point_v<U>>>
    constexpr Vec3<T> operator/(Vec3<T> vec, U num)
    {
      if (num)
	return Vec3<T>(vec.r / num, vec.g / num, vec.b / num);
      return Vec3<T>(T{},T{},T{});
    }
    // Overloading the output operator
    template <typename T>
    std::ostream& operator<<(std::ostream& os, Vec3<T> vec)
    {
	return os << "\nr ->  " << vec.r << "  g -> " << vec.g << "  b -> " << vec.b << std::endl;
    }
}

#endif 
