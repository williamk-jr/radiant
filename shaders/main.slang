struct VertexInput {
  float3 position;
  float3 normal;
  float2 uv;
};

struct VertexOutput {
  float3 position;
  float3 normal;
  float2 uv;
};

[shader("vertex")]
VertexOutput main(VertexInput vertex) {
  return {vertex.position, vertex.normal, vertex.uv};
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
  return {0.0, 0.0, 0.0, 1.0};
}
