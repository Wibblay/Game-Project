#version 330 core

layout(location = 0) in vec2 aPos;          // Vertex position (tile corners)
layout(location = 1) in vec2 aTilePos;      // Tile grid position
layout(location = 2) in float aHeight;      // Tile height
layout(location = 3) in vec3 aColor;        // Tile color
layout(location = 4) in float aRequiresShift;  // Whether the vertex needs shifting to tile height
layout(location = 5) in float aTopFaceFlag; // 1 if top face vertex

out vec3 fragColor;

uniform mat4 projection;
uniform mat2 transformMatrix;           
uniform float tileSize;
uniform vec2 cameraPosition;

void main()
{
    vec2 transformedPos = transformMatrix * (aTilePos - cameraPosition);
    
    // Calculate depth based on tile coordinates
    float depth = transformedPos.y * -1.0;

    vec2 shiftedPos = aPos + (aRequiresShift * vec2(0, aHeight));

    // Add scaled vertex position
    vec2 finalPos2D = ((transformedPos + shiftedPos) * tileSize);

    // Incorporate height as Z-coordinate
    vec3 finalPos = vec3(finalPos2D, depth);

    // Apply view and projection matrices
    gl_Position = projection * vec4(finalPos, 1.0);

    if (aTopFaceFlag < 0.5)
    {
        fragColor = aColor + vec3(0.0, 1.0, 0.0);
    }
    else
    {
        fragColor = aColor;
    }
}
