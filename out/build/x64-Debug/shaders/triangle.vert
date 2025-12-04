#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in vec3 a_Tangent;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_WorldPos;
out vec2 v_UV;
out mat3 v_TBN;

void main()
{
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;

    // Basis vectors
    vec3 N = normalize(mat3(u_Model) * a_Normal);
    vec3 T = normalize(mat3(u_Model) * a_Tangent);
    vec3 B = cross(N, T);

    v_TBN = mat3(T, B, N);
    v_UV = a_UV;

    gl_Position = u_Projection * u_View * worldPos;
}
