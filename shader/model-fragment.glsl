//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoords;
//in vec3 Normal;
//out vec3 FragPos;
//
////struct DirectionalLight {
////    vec3 direction;
////    vec3 ambient;
////    vec3 diffuse;
////    vec3 specular;
////};
//
//struct SpotLight {
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//uniform SpotLight light;
////uniform vec3 viewPos;
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//
//void main(){
//    vec3 lightDir = normalize(light.position - FragPos);
//    float theta = dot(lightDir, normalize(-light.direction));
//
//    vec4 diffuseTexture = texture(texture_diffuse1, TexCoords);
//    //vec4 specularTexture = texture(texture_specular1, TexCoords);
//    vec3 ambient = light.ambient * diffuseTexture.rgb;
//    vec4 color;
//
//    if (theta > light.cutOff){ // values are in cosines
//        vec3 norm = normalize(Normal);
//        float diff = max(dot(norm, lightDir), 0.0);
//        vec3 diffuse = light.diffuse * diff * diffuseTexture.rgb;
//        color = vec4(diffuse + ambient, 1.0);
//
//        //vec3 viewDir = normalize(viewPos - FragPos);
//        //vec3 reflectDir = reflect(-lightDir, norm);
//        //float specFloat = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // shine is 32.0 for now, update later
//        //vec3 specular = light.specular * specFloat * texture(texture_specular1, TexCoords).rgb;
//    }
//    else {
//        color = vec4(ambient, 1.0);
//    }
//    FragColor = color;
//    //FragColor = diffuseTexture + specularTexture;
//    //vec3 ambient = light.ambient * diffuseTexture.rgb;
//
//    //vec3 norm = normalize(Normal);
//    //vec3 lightDir = normalize(-light.direction);
//    //vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * diffuseTexture.rgb;
//
//    //vec3 viewDir = normalize(viewPos - FragPos);
//    //vec3 reflectDir = reflect(-lightDir, norm);
//    //float specFloat = pow(max(dot(viewDir, reflectDir), 0.0), 8.0); // shine is 32.0 for now, update later
//    //vec3 specular = light.specular * specFloat * texture(texture_specular1, TexCoords).rgb;
//
//    //FragColor = vec4(ambient + diffuse + specular, 1.0);
//}

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
//in vec3 Normal;
//out vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
    vec4 diffuse = texture(texture_diffuse1, TexCoords);
    //vec4 specular = texture(texture_specular1, TexCoords);
    //vec4 color = vec4(0.9, 0.9, 0.9, 1.0);
    FragColor = diffuse;
    //FragColor = color;
}
