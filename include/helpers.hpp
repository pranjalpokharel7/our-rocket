#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "../Maths/vec.hpp"
#include <glad/glad.h>

void update_uniform_3f(const char *uniform_name, unsigned int program_ID,
                       FMath::Vec3<float> in_vec3);
void update_uniform_1f(const char *uniform_name, unsigned int program_ID,
                       float uniform_value);
void update_uniform_matrix_4f(const char *uniform_name, unsigned int program_ID,
                       float* first_element_address);
void update_uniform_1i(const char *uniform_name, unsigned int program_ID,
                       int uniform_value);

// color management

FMath::Vec3<float> ambient(FMath::Vec3<float> color);
FMath::Vec3<float> diffuse(FMath::Vec3<float> color);
FMath::Vec3<float> specular(FMath::Vec3<float> color);


// will have to arrange all colors in an array and use an enum
// this has too many declarations, for ease, might remove later

//namespace Color {
//    FMath::Vec3<float>aliceblue = {0.941f, 0.973f, 1.000f};
//    FMath::Vec3<float>antiquewhite = {0.980f, 0.922f, 0.843f};
//    FMath::Vec3<float>aqua = {0.000f, 1.000f, 1.000f};
//    FMath::Vec3<float>aquamarine = {0.498f, 1.000f, 0.831f};
//    FMath::Vec3<float>azure = {0.941f, 1.000f, 1.000f};
//    FMath::Vec3<float>beige = {0.961f, 0.961f, 0.863f};
//    FMath::Vec3<float>bisque = {1.000f, 0.894f, 0.769f};
//    FMath::Vec3<float>black = {0.000f, 0.000f, 0.000f};
//    FMath::Vec3<float>blanchedalmond = {1.000f, 0.922f, 0.804f};
//    FMath::Vec3<float>blue = {0.000f, 0.000f, 1.000f};
//    FMath::Vec3<float>blueviolet = {0.541f, 0.169f, 0.886f};
//    FMath::Vec3<float>brown = {0.647f, 0.165f, 0.165f};
//    FMath::Vec3<float>burlywood = {0.871f, 0.722f, 0.529f};
//    FMath::Vec3<float>cadetblue = {0.373f, 0.620f, 0.627f};
//    FMath::Vec3<float>chartreuse = {0.498f, 1.000f, 0.000f};
//    FMath::Vec3<float>chocolate = {0.824f, 0.412f, 0.118f};
//    FMath::Vec3<float>coral = {1.000f, 0.498f, 0.314f};
//    FMath::Vec3<float>cornflowerblue = {0.392f, 0.584f, 0.929f};
//    FMath::Vec3<float>cornsilk = {1.000f, 0.973f, 0.863f};
//    FMath::Vec3<float>crimson = {0.863f, 0.078f, 0.235f};
//    FMath::Vec3<float>cyan = {0.000f, 1.000f, 1.000f};
//    FMath::Vec3<float>darkblue = {0.000f, 0.000f, 0.545f};
//    FMath::Vec3<float>darkcyan = {0.000f, 0.545f, 0.545f};
//    FMath::Vec3<float>darkgoldenrod = {0.722f, 0.525f, 0.043f};
//    FMath::Vec3<float>darkgray = {0.663f, 0.663f, 0.663f};
//    FMath::Vec3<float>darkgreen = {0.000f, 0.392f, 0.000f};
//    FMath::Vec3<float>darkgrey = {0.663f, 0.663f, 0.663f};
//    FMath::Vec3<float>darkkhaki = {0.741f, 0.718f, 0.420f};
//    FMath::Vec3<float>darkmagenta = {0.545f, 0.000f, 0.545f};
//    FMath::Vec3<float>darkolivegreen = {0.333f, 0.420f, 0.184f};
//    FMath::Vec3<float>darkorange = {1.000f, 0.549f, 0.000f};
//    FMath::Vec3<float>darkorchid = {0.600f, 0.196f, 0.800f};
//    FMath::Vec3<float>darkred = {0.545f, 0.000f, 0.000f};
//    FMath::Vec3<float>darksalmon = {0.914f, 0.588f, 0.478f};
//    FMath::Vec3<float>darkseagreen = {0.561f, 0.737f, 0.561f};
//    FMath::Vec3<float>darkslateblue = {0.282f, 0.239f, 0.545f};
//    FMath::Vec3<float>darkslategray = {0.184f, 0.310f, 0.310f};
//    FMath::Vec3<float>darkslategrey = {0.184f, 0.310f, 0.310f};
//    FMath::Vec3<float>darkturquoise = {0.000f, 0.808f, 0.820f};
//    FMath::Vec3<float>darkviolet = {0.580f, 0.000f, 0.827f};
//    FMath::Vec3<float>deeppink = {1.000f, 0.078f, 0.576f};
//    FMath::Vec3<float>deepskyblue = {0.000f, 0.749f, 1.000f};
//    FMath::Vec3<float>dimgray = {0.412f, 0.412f, 0.412f};
//    FMath::Vec3<float>dimgrey = {0.412f, 0.412f, 0.412f};
//    FMath::Vec3<float>dodgerblue = {0.118f, 0.565f, 1.000f};
//    FMath::Vec3<float>firebrick = {0.698f, 0.133f, 0.133f};
//    FMath::Vec3<float>floralwhite = {1.000f, 0.980f, 0.941f};
//    FMath::Vec3<float>forestgreen = {0.133f, 0.545f, 0.133f};
//    FMath::Vec3<float>fuchsia = {1.000f, 0.000f, 1.000f};
//    FMath::Vec3<float>gainsboro = {0.863f, 0.863f, 0.863f};
//    FMath::Vec3<float>ghostwhite = {0.973f, 0.973f, 1.000f};
//    FMath::Vec3<float>gold = {1.000f, 0.843f, 0.000f};
//    FMath::Vec3<float>goldenrod = {0.855f, 0.647f, 0.125f};
//    FMath::Vec3<float>gray = {0.502f, 0.502f, 0.502f};
//    FMath::Vec3<float>green = {0.000f, 0.502f, 0.000f};
//    FMath::Vec3<float>greenyellow = {0.678f, 1.000f, 0.184f};
//    FMath::Vec3<float>grey = {0.502f, 0.502f, 0.502f};
//    FMath::Vec3<float>honeydew = {0.941f, 1.000f, 0.941f};
//    FMath::Vec3<float>hotpink = {1.000f, 0.412f, 0.706f};
//    FMath::Vec3<float>indianred = {0.804f, 0.361f, 0.361f};
//    FMath::Vec3<float>indigo = {0.294f, 0.000f, 0.510f};
//    FMath::Vec3<float>ivory = {1.000f, 1.000f, 0.941f};
//    FMath::Vec3<float>khaki = {0.941f, 0.902f, 0.549f};
//    FMath::Vec3<float>lavender = {0.902f, 0.902f, 0.980f};
//    FMath::Vec3<float>lavenderblush = {1.000f, 0.941f, 0.961f};
//    FMath::Vec3<float>lawngreen = {0.486f, 0.988f, 0.000f};
//    FMath::Vec3<float>lemonchiffon = {1.000f, 0.980f, 0.804f};
//    FMath::Vec3<float>lightblue = {0.678f, 0.847f, 0.902f};
//    FMath::Vec3<float>lightcoral = {0.941f, 0.502f, 0.502f};
//    FMath::Vec3<float>lightcyan = {0.878f, 1.000f, 1.000f};
//    FMath::Vec3<float>lightgoldenrodyellow = {0.980f, 0.980f, 0.824f};
//    FMath::Vec3<float>lightgray = {0.827f, 0.827f, 0.827f};
//    FMath::Vec3<float>lightgreen = {0.565f, 0.933f, 0.565f};
//    FMath::Vec3<float>lightgrey = {0.827f, 0.827f, 0.827f};
//    FMath::Vec3<float>lightpink = {1.000f, 0.714f, 0.757f};
//    FMath::Vec3<float>lightsalmon = {1.000f, 0.627f, 0.478f};
//    FMath::Vec3<float>lightseagreen = {0.125f, 0.698f, 0.667f};
//    FMath::Vec3<float>lightskyblue = {0.529f, 0.808f, 0.980f};
//    FMath::Vec3<float>lightslategray = {0.467f, 0.533f, 0.600f};
//    FMath::Vec3<float>lightslategrey = {0.467f, 0.533f, 0.600f};
//    FMath::Vec3<float>lightsteelblue = {0.690f, 0.769f, 0.871f};
//    FMath::Vec3<float>lightyellow = {1.000f, 1.000f, 0.878f};
//    FMath::Vec3<float>lime = {0.000f, 1.000f, 0.000f};
//    FMath::Vec3<float>limegreen = {0.196f, 0.804f, 0.196f};
//    FMath::Vec3<float>linen = {0.980f, 0.941f, 0.902f};
//    FMath::Vec3<float>magenta = {1.000f, 0.000f, 1.000f};
//    FMath::Vec3<float>maroon = {0.502f, 0.000f, 0.000f};
//    FMath::Vec3<float>mediumaquamarine = {0.400f, 0.804f, 0.667f};
//    FMath::Vec3<float>mediumblue = {0.000f, 0.000f, 0.804f};
//    FMath::Vec3<float>mediumorchid = {0.729f, 0.333f, 0.827f};
//    FMath::Vec3<float>mediumpurple = {0.576f, 0.439f, 0.859f};
//    FMath::Vec3<float>mediumseagreen = {0.235f, 0.702f, 0.443f};
//    FMath::Vec3<float>mediumslateblue = {0.482f, 0.408f, 0.933f};
//    FMath::Vec3<float>mediumspringgreen = {0.000f, 0.980f, 0.604f};
//    FMath::Vec3<float>mediumturquoise = {0.282f, 0.820f, 0.800f};
//    FMath::Vec3<float>mediumvioletred = {0.780f, 0.082f, 0.522f};
//    FMath::Vec3<float>midnightblue = {0.098f, 0.098f, 0.439f};
//    FMath::Vec3<float>mintcream = {0.961f, 1.000f, 0.980f};
//    FMath::Vec3<float>mistyrose = {1.000f, 0.894f, 0.882f};
//    FMath::Vec3<float>moccasin = {1.000f, 0.894f, 0.710f};
//    FMath::Vec3<float>navajowhite = {1.000f, 0.871f, 0.678f};
//    FMath::Vec3<float>navy = {0.000f, 0.000f, 0.502f};
//    FMath::Vec3<float>oldlace = {0.992f, 0.961f, 0.902f};
//    FMath::Vec3<float>olive = {0.502f, 0.502f, 0.000f};
//    FMath::Vec3<float>olivedrab = {0.420f, 0.557f, 0.137f};
//    FMath::Vec3<float>orange = {1.000f, 0.647f, 0.000f};
//    FMath::Vec3<float>orangered = {1.000f, 0.271f, 0.000f};
//    FMath::Vec3<float>orchid = {0.855f, 0.439f, 0.839f};
//    FMath::Vec3<float>palegoldenrod = {0.933f, 0.910f, 0.667f};
//    FMath::Vec3<float>palegreen = {0.596f, 0.984f, 0.596f};
//    FMath::Vec3<float>paleturquoise = {0.686f, 0.933f, 0.933f};
//    FMath::Vec3<float>palevioletred = {0.859f, 0.439f, 0.576f};
//    FMath::Vec3<float>papayawhip = {1.000f, 0.937f, 0.835f};
//    FMath::Vec3<float>peachpuff = {1.000f, 0.855f, 0.725f};
//    FMath::Vec3<float>peru = {0.804f, 0.522f, 0.247f};
//    FMath::Vec3<float>pink = {1.000f, 0.753f, 0.796f};
//    FMath::Vec3<float>plum = {0.867f, 0.627f, 0.867f};
//    FMath::Vec3<float>powderblue = {0.690f, 0.878f, 0.902f};
//    FMath::Vec3<float>purple = {0.502f, 0.000f, 0.502f};
//    FMath::Vec3<float>red = {1.000f, 0.000f, 0.000f};
//    FMath::Vec3<float>rosybrown = {0.737f, 0.561f, 0.561f};
//    FMath::Vec3<float>royalblue = {0.255f, 0.412f, 0.882f};
//    FMath::Vec3<float>saddlebrown = {0.545f, 0.271f, 0.075f};
//    FMath::Vec3<float>salmon = {0.980f, 0.502f, 0.447f};
//    FMath::Vec3<float>sandybrown = {0.957f, 0.643f, 0.376f};
//    FMath::Vec3<float>seagreen = {0.180f, 0.545f, 0.341f};
//    FMath::Vec3<float>seashell = {1.000f, 0.961f, 0.933f};
//    FMath::Vec3<float>sienna = {0.627f, 0.322f, 0.176f};
//    FMath::Vec3<float>silver = {0.753f, 0.753f, 0.753f};
//    FMath::Vec3<float>skyblue = {0.529f, 0.808f, 0.922f};
//    FMath::Vec3<float>slateblue = {0.416f, 0.353f, 0.804f};
//    FMath::Vec3<float>slategray = {0.439f, 0.502f, 0.565f};
//    FMath::Vec3<float>slategrey = {0.439f, 0.502f, 0.565f};
//    FMath::Vec3<float>snow = {1.000f, 0.980f, 0.980f};
//    FMath::Vec3<float>springgreen = {0.000f, 1.000f, 0.498f};
//    FMath::Vec3<float>steelblue = {0.275f, 0.510f, 0.706f};
//    FMath::Vec3<float>tan = {0.824f, 0.706f, 0.549f};
//    FMath::Vec3<float>teal = {0.000f, 0.502f, 0.502f};
//    FMath::Vec3<float>thistle = {0.847f, 0.749f, 0.847f};
//    FMath::Vec3<float>tomato = {1.000f, 0.388f, 0.278f};
//    FMath::Vec3<float>turquoise = {0.251f, 0.878f, 0.816f};
//    FMath::Vec3<float>violet = {0.933f, 0.510f, 0.933f};
//    FMath::Vec3<float>wheat = {0.961f, 0.871f, 0.702f};
//    FMath::Vec3<float>white = {1.000f, 1.000f, 1.000f};
//    FMath::Vec3<float>whitesmoke = {0.961f, 0.961f, 0.961f};
//    FMath::Vec3<float>yellow = {1.000f, 1.000f, 0.000f};
//    FMath::Vec3<float>yellowgreen = {0.604f, 0.804f, 0.196f};
//};

#endif
