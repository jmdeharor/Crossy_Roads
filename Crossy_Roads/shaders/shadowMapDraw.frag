#version 330 core

uniform sampler2D tex;
uniform sampler2DShadow shadowMap;

uniform vec3 lightDir;

in vec4 shadowCoord;
in vec2 fragTexCoord;
in vec3 fragNormal;
out vec4 outColor;

float sampleShadow() {
	const float xOffset = 1.0/2048;
    const float yOffset = 1.0/2048;
	float factor = 0;
	
	for (int y = -2 ; y <= 2 ; y++) {
        for (int x = -2 ; x <= 2 ; x++) {
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            vec3 UVC = vec3(shadowCoord.st + Offsets, shadowCoord.z - 0.0009);
            factor += texture(shadowMap, UVC);
        }
    }
	
	return factor/25;
}

void main() {
	vec4 color = texture(tex, fragTexCoord);
	if (gl_FragCoord.z < 0.991 || color.a < 0.5)
		discard;
	vec3 N = normalize(fragNormal);
	vec2 st = shadowCoord.st;
	float trueDepth = shadowCoord.z;
	float diffuse = max(0, dot(N,lightDir));
	if (st.s < 0 || st.s > 1 || st.t < 0 || st.t > 1) {
		outColor = mix(0.5, 1, diffuse)*color;
	}
	else {
		float storedDepth = texture(shadowMap, vec3(st, trueDepth-0.0009));
		float finalFactor = min(diffuse, storedDepth);
		outColor = mix(0.5, 1, finalFactor)*color;
	}
}