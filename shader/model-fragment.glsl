
#version 330 core
#define NR_LIGHTS 9 // remember to re-update this everytime you change the number of light cubes

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct LightSource {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constantAtten;
    float linearAtten;
    float quadraticAtten;
};
uniform LightSource light[NR_LIGHTS];

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform sampler2D depthBuffer;
uniform sampler2D prevBuffer;

vec2 dimension = vec2(1000.0f,1000.0f);

void main(){

    vec4 color = texture(depthBuffer,gl_FragCoord.xy/dimension);
    float prev_depth = color.x;
    float new_depth = pow(gl_FragCoord.z, 100);

    if (new_depth <= prev_depth + 0.002f )
    { 

      vec3 textureDiffuse = texture(texture_diffuse1, TexCoords).rgb;
      vec3 textureSpecular = texture(texture_diffuse1, TexCoords).rgb;
      vec3 result = vec3(0.0, 0.0, 0.0);
      vec3 norm = normalize(Normal);
      vec3 viewDir = normalize(viewPos - FragPos);
      
      for (int i = 0; i < NR_LIGHTS; i++){
         // ambient
     	    vec3 ambient = light[i].ambient * textureDiffuse;
	    
            // diffuse
            vec3 lightDir = normalize(light[i].position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light[i].diffuse * diff * textureDiffuse;

	            // specular
        	   //  vec3 reflectDir = reflect(-lightDir, norm);
		   vec3 reflectDir = normalize(lightDir + viewDir);

	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0); // hard-coded shininess for now
	    vec3 specular = light[i].specular * (spec * textureSpecular); // our material does not have a specular tecture yet

       	     // attenuation
       	      float dist = length(light[i].position - FragPos);
              float attenuation = 1.0 / (light[i].constantAtten + light[i].linearAtten * dist + light[i].quadraticAtten * dist * dist);
              ambient *= attenuation;
              diffuse *= attenuation;
              specular *= attenuation;

        // output
	      result += (ambient + diffuse + specular);
   	       }
   	       FragColor = vec4(result, 1.0);
    }
    else
	FragColor = texture(prevBuffer,gl_FragCoord.xy/(dimension));
}
