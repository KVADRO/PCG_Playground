#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 vTexCoord;

uniform mat4 uM;
uniform mat4 uVP;
uniform sampler2D uHeightmap;

void
main()
{
  vec4 height = texture(uHeightmap, inTexCoord);
  gl_Position = uVP * uM * vec4(inPos.x, height.r * 20.0, inPos.z, 1.0);

  vTexCoord = inTexCoord;
}