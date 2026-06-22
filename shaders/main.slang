struct VertexInput {
  float3 position;
  float3 normal;
  float2 uv;

  float4 color;
  float3 screenPosition;
  float2 screenSize;
};

struct VertexOutput {
  float4 position : SV_Position;
  float3 normal;
  float2 uv;
  
  float4 color;
  float3 screenPosition;
  float2 screenSize;
};

struct ShaderData {
  uint32_t2 frameBufferSize;
};

uniform ShaderData shaderData;

[shader("vertex")]
VertexOutput main(VertexInput vertex, uint instanceId : SV_InstanceID) {
  float2 normalizedVertexPosition = (vertex.position.xy + 1) / 2;

  float sizeX = vertex.screenSize.x * normalizedVertexPosition.x;
  float sizeY = vertex.screenSize.y * normalizedVertexPosition.y;
  
  float newX = ( (vertex.screenPosition.x + sizeX) / (float)shaderData.frameBufferSize.x) * 2.0 - 1.0;
  float newY = ( (vertex.screenPosition.y + sizeY) / (float)shaderData.frameBufferSize.y) * 2.0 - 1.0;

  return {
    float4(newX, newY, vertex.position.z, 1.0), vertex.normal, vertex.uv,
    vertex.color, vertex.screenPosition, vertex.screenSize
  };
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  return normalize(vertex.color);
  //return float4(vertex.uv.x, vertex.uv.y, 0.0, 1.0);
}
