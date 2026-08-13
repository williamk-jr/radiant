struct VertexOutput {
		float4 position : SV_Position;
		float2 uv;

		float4 color;
		float2 screenPosition;
		float2 screenSize;
		float2 uvMin;
		float2 uvMax;
};

struct ShaderData {
		float4x4 orthoMatrix;
};

uniform ShaderData shaderData;

[[vk::binding(0, 1)]]
Sampler2D textureSampler;

[shader("fragment")]
float4 main(VertexOutput vertex) {
	float4 color = textureSampler.Sample(vertex.uv);
	if (color.a == 0)
		discard;
	return float4(color.r + vertex.color.r, color.g + vertex.color.g, color.b + vertex.color.b, color.a);
}

