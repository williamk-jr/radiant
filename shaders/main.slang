struct VertexInput {
  float2 position;
  float2 uv;

  float4 color;
  float2 screenPosition;
  float2 screenSize;
};

struct VertexOutput {
  float4 position : SV_Position;
  float2 uv;
  
  float4 color;
  float2 screenPosition;
  float2 screenSize;
};

struct ShaderData {
  float4x4 orthoMatrix;
};

uniform ShaderData shaderData;

[shader("vertex")]
VertexOutput main(VertexInput vertex, uint instanceId : SV_InstanceID) {
  float2 normalizedVertexPosition = (vertex.position.xy + 1) / 2;
  
  float2 size = vertex.screenSize * normalizedVertexPosition;
  float2 vertexPos = size + vertex.screenPosition;
  float4 pos = mul(shaderData.orthoMatrix, float4(vertexPos, 0.0, 1.0));

  float4 normalizedColor = vertex.color / float4(255);
  float4 premultipliedColor = float4(normalizedColor.rgb*normalizedColor.a, normalizedColor.a);

  return {
    pos, vertex.uv,
    premultipliedColor, vertex.screenPosition, vertex.screenSize
  };
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  float borderRadius = 1.0;

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

  return vertex.color;
}
