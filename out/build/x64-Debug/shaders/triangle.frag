#version 330 core

in vec3 v_WorldPos;
in vec2 v_UV;
in mat3 v_TBN;

out vec4 FragColor;

// Material
struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    int diffuseTexture; // kept for compatibility
};

uniform Material u_Material;

// Normal map
uniform sampler2D u_NormalMap;
uniform int u_NormalMapEnabled;

// Diffuse texture
uniform sampler2D u_TextureSampler;

// Directional light
struct DirectionalLight {
    vec3 color;
    float intensity;
};
uniform int u_HasDirectionalLight;
uniform DirectionalLight u_DirectionalLight;

// Point lights
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

#define MAX_POINT_LIGHTS 8
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

// Camera
uniform vec3 u_ViewPos;

void main()
{
    // Normal mapping
    vec3 N;
    if (u_NormalMapEnabled == 1)
    {
        vec3 n = texture(u_NormalMap, v_UV).xyz * 2.0 - 1.0;
        N = normalize(v_TBN * n);
    }
    else
    {
        N = normalize(v_TBN[2]);
    }

    vec3 V = normalize(u_ViewPos - v_WorldPos);
    vec3 result = vec3(0.0);

    // Directional light
    if (u_HasDirectionalLight == 1)
    {
        vec3 L = normalize(vec3(0.0, -1.0, -1.0));
        float diff = max(dot(N, -L), 0.0);

        vec3 diffuse  = diff * u_DirectionalLight.color * u_DirectionalLight.intensity;

        vec3 H = normalize(-L + V);
        float spec = pow(max(dot(N, H), 0.0), u_Material.shininess);
        vec3 specular = spec * u_Material.specularColor * u_DirectionalLight.intensity;

        result += diffuse + specular;
    }

    // Point lights
    for (int i = 0; i < u_PointLightCount; i++)
    {
        vec3 L = normalize(u_PointLights[i].position - v_WorldPos);
        float diff = max(dot(N, L), 0.0);

        vec3 diffuse = diff * u_PointLights[i].color * u_PointLights[i].intensity;

        vec3 H = normalize(L + V);
        float spec = pow(max(dot(N, H), 0.0), u_Material.shininess);
        vec3 specular = spec * u_Material.specularColor * u_PointLights[i].intensity;

        float distance    = length(u_PointLights[i].position - v_WorldPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        result += (diffuse + specular) * attenuation;
    }

    vec3 baseColor = u_Material.diffuseColor;

    if (u_Material.diffuseTexture >= 0)
        baseColor *= texture(u_TextureSampler, v_UV).rgb;

    result *= baseColor;

    FragColor = vec4(result, 1.0);
}
