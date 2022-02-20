#version 330 core

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D uTexture;
uniform sampler2D uKdTexture;
uniform sampler2D uKsTexture;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;
uniform vec3 uLightPos_vs2;
uniform vec3 uLightIntensity2;

out vec3 fFragColor;

vec3 blinnPhong(vec3 position_vs, vec3 normal_vs){
	vec3 kd = texture(uKdTexture, vTexCoords).rgb;
    vec3 ks = texture(uKsTexture, vTexCoords).rgb;
	vec3 w_zero = normalize(-position_vs);
	vec3 w_i = normalize(uLightPos_vs - vPosition_vs);
	vec3 halfVector = (w_zero + w_i) / 2;

    float d = distance(uLightPos_vs,vPosition_vs);
    vec3 LightIntensity = (uLightIntensity / (d * d));

    float d2 = distance(uLightPos_vs2,vPosition_vs);
    vec3 LightIntensity2 = (uLightIntensity2 / (d2 * d2));

    vec3 light1 = LightIntensity * ( kd * ( dot(w_i, normal_vs ) ) + ks * ( pow( dot(halfVector, normal_vs), uShininess ) ) );
    vec3 light2 = LightIntensity2 * ( kd * ( dot(w_i, normal_vs ) ) + ks * ( pow( dot(halfVector, normal_vs), uShininess ) ) );

	return light1+light2;
}

void main() {

    //vec4 color = texture(uTexture, vTexCoords);
    //vec4 color2 = texture(uTexture2, vTexCoords);
    //fFragColor = vec3(1,0,0);
    fFragColor = blinnPhong(vPosition_vs, normalize(vNormal_vs));
    //fFragColor = texture(uTexture,vTexCoords).rgb;
}