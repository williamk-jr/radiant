struct VertexInput {
		float2 position;
		float2 uv;

		float4 color;
		float2 screenPosition;
		float2 screenSize;
		float2 uvMin;
		float2 uvMax;
};

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
	float4 normalized = color / float4(255);
	return float4(normalized.rgb * normalized.a, normalized.a);
}

bool point2DWithinBounds(float2 point, float2 min, float2 max) {
	return (min.x <= point.x && point.x <= max.x) && (min.y <= point.y && point.y <= max.y);
}

[shader("vertex")]
VertexOutput main(VertexInput vertex, uint instanceId: SV_InstanceID) {
	// Transform model-space
	float2 scaledPosition     = scale2D(vertex.position.xy, vertex.screenSize);
	float2 translatedPosition = translate2D(scaledPosition, vertex.screenPosition);
	float4 clipPosition       = toClip(translatedPosition);

	float2 quadCorner = (vertex.position + 1.0) / 2.0;
	float2 uv         = lerp(vertex.uvMin, vertex.uvMax, quadCorner);

	return { clipPosition, uv,          premultiplyAlpha(vertex.color), vertex.screenPosition, vertex.screenSize,
		     vertex.uvMin, vertex.uvMax };
}

[shader("fragment")]
float4 main(VertexOutput vertex) {
	return vertex.color;
}


