#include "vec.hpp"
#include "Matrix.hpp"
#include <iostream>

// some template to reveal its value now

int main()
{
	constexpr auto value = FMath::NewtonSqrt(1000);
	auto xvalue = FMath::FastInverseSqrt(1000);
	std::cout << "Difference in their value was : " << value - 1.0f/xvalue << std::endl;

	auto c = FMath::Vec3<float>(1,2,4);
	std::cout << c * 1.25f;
	auto d = FMath::Mat4<float>(1.0f);
	std::cout << d << std::endl;
    // auto f = c.unitVec();
	// std::cout << "Result is : " <<  f.norm();
    return 0;
	
}
