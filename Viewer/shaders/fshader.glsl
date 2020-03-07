#version 330 core

uniform vec4 ambiantColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

// Camera
uniform vec3 cameraLocation;

// Lights
uniform vec4 ambiantLighting;
uniform int numberOfLights;
uniform vec3 lightsPositions[20];
uniform vec4 lightColors[20];


// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	vec4 diffuse =  vec4(0.0f);
	vec4 specular = vec4(0.0f);
	float dotProd = 0.0f;
	
	for(int i = 0; i < numberOfLights; i++)
	{
		//diffuse
		vec3 lightDir = normalize(lightsPositions[i] - fragPos);
		dotProd = dot(lightDir, normalize(fragNormal));

		float diff1 = (dotProd > 0.0f) ? dotProd : 0.0f;
		diffuse = diffuse + diffuseColor * diff1 * lightColors[i];


		// specular
		vec3 viewDir = normalize(normalize(fragPos) - normalize(vec3(cameraLocation)));
		vec3 reflectDir = normalize(reflect(normalize(-lightDir), normalize(fragNormal)));
		dotProd = dot(reflectDir, viewDir);
		float diff = (dotProd > 0.0f) ? dotProd : 0.0f;
		float spec = pow(abs(diff), shininess);
		specular = specular + specularColor * spec * lightColors[i];
	}

	frag_color = (ambiantColor * ambiantLighting  + diffuse + specular);

	frag_color.w = 1.0f;
}
