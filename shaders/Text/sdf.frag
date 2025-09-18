#version 450
layout(set = 0, binding = 0) uniform sampler2D sdfAtlas;

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

void main() {
    float dist = texture(sdfAtlas, fragUV).r;
    float alpha = smoothstep(0.5 - 0.05, 0.5 + 0.05, dist);
    outColor = vec4(1.0, 1.0, 1.0, alpha);
}
