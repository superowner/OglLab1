#version 410 core

uniform vec3 LightAmbient;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform int enableDirectLight;
uniform float LightShine;
uniform float LightStrength;
uniform vec4 Color;

in vec4 mvPos;
in vec3 N;
out vec4 color;

void main()
{
if(enableDirectLight==1)
	{
		vec3 L = normalize(LightPos - mvPos.xyz); //light direction
		vec3 E = -normalize(mvPos.xyz);
		vec3 H = normalize(E+L);
		float diffuse = max(0.0, dot(N, L));
		float specular = max(0.0, dot(N, H));

		if(diffuse==0.0)
			specular = 0.0;
		else
			specular = pow(specular, LightShine);
		vec3 scatteredLight = LightAmbient+LightColor*diffuse;
		vec3 reflectedLight = LightColor*specular*LightStrength;
		vec3 _color = min(Color.rgb*scatteredLight+reflectedLight, vec3(1.0));
		color=vec4(_color, Color.a);
	}
	else if (enableDirectLight==0)
		{
		vec3 _color = min(Color.rgb*LightColor, vec3(1.0));
		color=vec4(_color, 1);
		}

}