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

bool point2DWithinBounds(float2 point, float2 min, float2 max) {
  return (min.x <= point.x && point.x <= max.x) &&
         (min.y <= point.y && point.y <= max.y);
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
  bool outsideCircularMask = distance(bounds,float2(maskCenter)) > borderRadius;
  if (outsideCircularMask && (bounds.x > 1.0-borderRadius) && (bounds.y > 1.0-borderRadius)) {
    discard;
  }
  
  // Fragment position starts as pixel coordinates, so we convert them to clip-space coordinates.
  float2 clipFragPos = toClip(vertex.position.xy).xy;
  if (point2DWithinBounds(clipFragPos, vertex.screenMin, vertex.screenMax)) { // Check if fragment is in content box.

    // Create UV coordinates for content box.
    float2 contentUV = float2(
      (clipFragPos.x-vertex.screenMin.x) / (vertex.screenMax.x-vertex.screenMin.x),
      (clipFragPos.y-vertex.screenMin.y) / (vertex.screenMax.y-vertex.screenMin.y)
    );
    float2 absoluteContentBounds = abs(contentUV-0.5)*2.0;

    // Check if pixel is in any of the 4 corners with side length borderRadius*2
    if (!point2DWithinBounds(absoluteContentBounds, 1.0-(borderRadius*2), 1.0)) {
      return vertex.color;
    } else {
      if ((distance(absoluteContentBounds, float2(1.0-borderRadius)) < borderRadius) ||
          (absoluteContentBounds.x < 1.0-borderRadius) || (absoluteContentBounds.y < 1.0-borderRadius)) {
        return vertex.color;
      }
    }
  }


  return vertex.borderColor;
}
