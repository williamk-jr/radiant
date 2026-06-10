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

[shader("vertex")]
VertexOutput main(VertexInput vertex, uint instanceId : SV_InstanceID) {
  return {
    float4(vertex.position, 1.0), vertex.normal, vertex.uv,
    vertex.color, vertex.screenPosition, vertex.screenSize
  };
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  return normalize(vertex.color);
  //return float4(vertex.uv.x, vertex.uv.y, 0.0, 1.0);
}
