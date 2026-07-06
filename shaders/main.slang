struct VertexInput {
  float2 position;
  float2 uv;

  float4 color;
  float2 screenPosition;
  float2 screenSize;

  float4 borderColor;
  float2 borderPosition;
  float2 borderSize;
};

struct VertexOutput {
  float4 position : SV_Position;
  float2 uv;
  
  float4 color;
  float2 screenMin;
  float2 screenMax;

  float4 borderColor;
};

struct ShaderData {
  float4x4 orthoMatrix;
};

uniform ShaderData shaderData;

float2 scale2D(float2 vertexPos, float2 size) {
  float2 normalizedVertexPosition = (vertexPos + 1) / 2;
  return size * normalizedVertexPosition;
}

float2 translate2D(float2 vertexPos, float2 newVertexPosition) {
  return vertexPos + newVertexPosition;
}

float4 toClip(float2 vertexPos) {
  return mul(shaderData.orthoMatrix, float4(vertexPos, 0.0, 1.0));
}

float4 premultiplyAlpha(float4 color) {
  float4 normalized = color/float4(255);
  return float4(normalized.rgb*normalized.a, normalized.a);
}

[shader("vertex")]
VertexOutput main(VertexInput vertex, uint instanceId : SV_InstanceID) {
  // Transform model-space 
  float2 scaledPosition = scale2D(vertex.position.xy, vertex.borderSize);
  float2 translatedPosition = translate2D(scaledPosition, vertex.borderPosition);
  float4 clipPosition = toClip(translatedPosition);

  float4 normalizedColor = vertex.color / float4(255);
  float4 premultipliedColor = float4(normalizedColor.rgb*normalizedColor.a, normalizedColor.a);

  return {
    clipPosition, vertex.uv,
    premultiplyAlpha(vertex.color), toClip(vertex.screenPosition).xy, toClip(vertex.screenPosition+vertex.screenSize).xy,
    premultiplyAlpha(vertex.borderColor)
  };
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  float borderRadius = 0.1;
  float2 bounds = abs(vertex.uv-0.5)*2.0;

  // Where 1.0 is the maximum value in any given direction;
  float maskCenter = 1.0-borderRadius;

  // The area to discard must be:
  // 1. Outside the circle mask at each corner.
  // 2. X and Y bounds of the quad must both be greater than
  //    the center of the circle mask.
  if ((distance(bounds, float2(maskCenter)) > borderRadius) &&
      (bounds.x > 1.0-borderRadius) && (bounds.y > 1.0-borderRadius)) {
    discard;
  }

  float2 clipFragPos = toClip(vertex.position.xy).xy;
  float2 s = step(vertex.screenMin, clipFragPos) - step(vertex.screenMax, clipFragPos);

  if ((s.x*s.y) > 0.5) {
    return vertex.color;
  }


  return vertex.borderColor;
}
