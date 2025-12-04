#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 FragColor;

// Material
struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    int diffuseTexture;
};

uniform Material u_Material;

// Directional light
struct DirectionalLight {
    vec3 color;
    float intensity;
};
uniform int u_HasDirectionalLight;
uniform DirectionalLight u_DirectionalLight;

// Point light
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

#define MAX_POINT_LIGHTS 8
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

// Texture
uniform sampler2D u_TextureSampler;

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(-v_FragPos); // Assuming camera at origin

    vec3 resultColor = vec3(0.0);

    //----------------------------------------------------
    // Directional Lighting
    //----------------------------------------------------
    if (u_HasDirectionalLight == 1)
    {
        vec3 lightDir = normalize(vec3(0.0, -1.0, -1.0)); // default dir
        float diff = max(dot(normal, -lightDir), 0.0);

        vec3 diffuse  = diff * u_DirectionalLight.color * u_DirectionalLight.intensity;
        vec3 reflectDir = reflect(lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = spec * u_Material.specularColor * u_DirectionalLight.intensity;

        resultColor += diffuse + specular;
    }

    //----------------------------------------------------
    // Point lights
    //----------------------------------------------------
    for (int i = 0; i < u_PointLightCount; i++)
    {
        vec3 lightDir = normalize(u_PointLights[i].position - v_FragPos);
        float diff = max(dot(normal, lightDir), 0.0);

        // Diffuse
        vec3 diffuse = diff * u_PointLights[i].color * u_PointLights[i].intensity;

        // Specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = spec * u_Material.specularColor * u_PointLights[i].intensity;

        // Attenuation (simple ¡ª improve later if needed)
        float distance    = length(u_PointLights[i].position - v_FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        resultColor += (diffuse + specular) * attenuation;
    }

    //----------------------------------------------------
    // Final Material Diffuse handling
    //----------------------------------------------------
    vec3 baseColor = u_Material.diffuseColor;

    if (u_Material.diffuseTexture >= 0)
    {
        vec3 texColor = texture(u_TextureSampler, v_TexCoord).rgb;
        baseColor *= texColor;
    }

    resultColor *= baseColor;

    FragColor = vec4(resultColor, 1.0);
}
