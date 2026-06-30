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
  float4x4 orthoMatrix;
};

uniform ShaderData shaderData;

[shader("vertex")]
VertexOutput main(VertexInput vertex, uint instanceId : SV_InstanceID) {
  float2 normalizedVertexPosition = (vertex.position.xy + 1) / 2;
  
  float2 size = vertex.screenSize * normalizedVertexPosition;
  float2 vertexPos = size + vertex.screenPosition.xy;

  float4 pos = mul(shaderData.orthoMatrix, float4(vertexPos, 0.0, 1.0));
  float4 normalizedColor = vertex.color / float4(255);
  float4 premultipliedColor = float4(normalizedColor.rgb*normalizedColor.a, normalizedColor.a);

  return {
    pos, vertex.normal, vertex.uv,
    premultipliedColor, vertex.screenPosition, vertex.screenSize
  };
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  return vertex.color;
}
