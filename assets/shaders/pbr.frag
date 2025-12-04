#version 330 core

in vec3 v_WorldPos;
in vec2 v_UV;
in mat3 v_TBN;

out vec4 FragColor;

// =============================================================
// Material textures
// =============================================================
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_DisplacementMap;

// Enable flags
uniform int u_AlbedoMapEnabled;
uniform int u_NormalMapEnabled;
uniform int u_RoughnessMapEnabled;
uniform int u_MetalnessMapEnabled;
uniform int u_DisplacementMapEnabled;

// =============================================================
// Material fallback values (Task 7)
// =============================================================
uniform vec3  u_Material_DefaultAlbedo;   // NEW
uniform float u_DefaultRoughness;          // NEW
uniform float u_DefaultMetalness;          // NEW

// Legacy Phong metadata (kept for consistency)
uniform vec3  u_Material_diffuseColor;
uniform vec3  u_Material_specularColor;
uniform float u_Material_shininess;

// =============================================================
// Lighting definitions (unchanged)
// =============================================================
struct DirectionalLight {
    vec3 color;
    float intensity;
};
uniform int u_HasDirectionalLight;
uniform DirectionalLight u_DirectionalLight;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};
#define MAX_POINT_LIGHTS 8
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

uniform vec3 u_ViewPos;

// =============================================================
// Get Normal
// =============================================================
vec3 GetNormal()
{
    if (u_NormalMapEnabled == 1)
    {
        vec3 n = texture(u_NormalMap, v_UV).xyz * 2.0 - 1.0;
        return normalize(v_TBN * n);
    }

    return normalize(v_TBN[2]); 
}

// =============================================================
// Parallax Mapping
// =============================================================
vec2 ParallaxMapping(vec2 uv, vec3 viewDir)
{
    if (u_DisplacementMapEnabled == 0)
        return uv;

    float height = texture(u_DisplacementMap, uv).r;
    float scale = 0.04;
    float bias  = -0.02;

    float offset = height * scale + bias;
    return uv + viewDir.xy * offset;
}

// =============================================================
// Fresnel Schlick
// =============================================================
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// =============================================================
// GGX NDF
// =============================================================
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.141592 * denom * denom;

    return num / denom;
}

// =============================================================
// Geometry (Smith)
// =============================================================
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// =============================================================
// Main
// =============================================================
void main()
{
    // View direction
    vec3 V = normalize(u_ViewPos - v_WorldPos);

    // Parallax-adjusted UV
    vec2 uv = ParallaxMapping(v_UV, normalize(v_TBN * V));

    // Normal
    vec3 N = GetNormal();

    // =============================================================
    // ALBEDO (Task 7 fallback)
    // =============================================================
    vec3 albedo;
    if (u_AlbedoMapEnabled == 1)
        albedo = texture(u_AlbedoMap, uv).rgb;
    else
        albedo = u_Material_DefaultAlbedo;

    // =============================================================
    // ROUGHNESS (Task 7 fallback)
    // =============================================================
    float roughness;
    if (u_RoughnessMapEnabled == 1)
        roughness = texture(u_RoughnessMap, uv).r;
    else
        roughness = u_DefaultRoughness;

    // =============================================================
    // METALNESS (Task 7 fallback)
    // =============================================================
    float metalness;
    if (u_MetalnessMapEnabled == 1)
        metalness = texture(u_MetalnessMap, uv).r;
    else
        metalness = u_DefaultMetalness;

    // =============================================================
    // F0 (base reflectivity)
    // =============================================================
    vec3 F0 = mix(vec3(0.04), albedo, metalness);

    vec3 Lo = vec3(0.0);

    // =============================================================
    // Directional Light
    // =============================================================
    if (u_HasDirectionalLight == 1)
    {
        vec3 L = normalize(vec3(0.0, -1.0, -1.0));
        vec3 H = normalize(V + L);

        float NdotL = max(dot(N, L), 0.0);

        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3  F   = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nom = NDF * G * F;
        float denom = 4.0 * max(dot(N,V), 0.0) * NdotL + 0.001;

        vec3 spec = nom / denom;

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - metalness);

        Lo += (kD * albedo / 3.141592 + spec)
            * u_DirectionalLight.intensity
            * u_DirectionalLight.color
            * NdotL;
    }

    // =============================================================
    // Point Lights
    // =============================================================
    for (int i = 0; i < u_PointLightCount; i++)
    {
        vec3 L = normalize(u_PointLights[i].position - v_WorldPos);
        vec3 H = normalize(V + L);

        float NdotL = max(dot(N, L), 0.0);

        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3  F   = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nom = NDF * G * F;
        float denom = 4.0 * max(dot(N,V),0.0) * NdotL + 0.001;

        vec3 spec = nom / denom;

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - metalness);

        float dist = length(u_PointLights[i].position - v_WorldPos);
        float atten =
            1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

        vec3 radiance =
            u_PointLights[i].color * u_PointLights[i].intensity * atten;

        Lo += (kD * albedo / 3.141592 + spec)
            * radiance * NdotL;
    }

    FragColor = vec4(Lo, 1.0);
}
