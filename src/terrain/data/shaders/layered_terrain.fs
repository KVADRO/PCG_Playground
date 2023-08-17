#version 330 core

in vec2 vTexCoord;

out vec4 outColor;

struct Layer
{
  sampler2D layerTexture;

  float minHeight;
  float maxHeight;
};

uniform Layer uLayers[4];
uniform sampler2D uHeightmap;

void
main()
{
  float height = texture(uHeightmap, vTexCoord).r;

  int bottomLayerIdx = 0;
  for (; bottomLayerIdx < 4; ++bottomLayerIdx) {
    if (height >= uLayers[bottomLayerIdx].minHeight &&
        height <= uLayers[bottomLayerIdx].maxHeight)
      break;
  }

  if (bottomLayerIdx < 4) {
    vec2 scaledUV = vTexCoord * 3.0;
    outColor = texture(uLayers[bottomLayerIdx].layerTexture, scaledUV);

    float minHeight = uLayers[bottomLayerIdx].minHeight;
    float maxHeight = uLayers[bottomLayerIdx].maxHeight;

    for (int i = bottomLayerIdx + 1; i < 4; ++i) {
      if (height < uLayers[i].minHeight || height > uLayers[i].maxHeight)
        continue;

      float overlapMinHeight = max(minHeight, uLayers[i].minHeight);
      float overlapMaxHeight = min(maxHeight, uLayers[i].maxHeight);

      float alpha =
        (height - overlapMinHeight) / (overlapMaxHeight - overlapMinHeight);

      outColor =
        mix(outColor, texture(uLayers[i].layerTexture, scaledUV), alpha);

      minHeight = uLayers[i].minHeight;
      maxHeight = uLayers[i].maxHeight;
    }
  }
}