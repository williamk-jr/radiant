struct VertexInput {
  float3 position;
  float3 normal;
  float2 uv;
};

struct VertexOutput {
  float4 position : SV_Position;
  float3 normal;
  float2 uv;
};

[shader("vertex")]
VertexOutput main(VertexInput vertex) {
  return {float4(vertex.position, 1.0), vertex.normal, vertex.uv};
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  return float4(vertex.uv.x, vertex.uv.y, 0.0, 1.0);
}
