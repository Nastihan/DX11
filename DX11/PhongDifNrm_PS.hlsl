#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"

#include "PointLight.hlsli"
#include "Pshadow.hlsli"

cbuffer ObjectCBuf : register(b1)
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    bool useNormalMap;
    float normalMapWeight;
};
struct PS_Input
{
    float3 viewFragPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTan : TANGENT;
    float3 viewBitan : BITANGENT;
    float2 tc : TEXCOORD;
    float4 spos : ShadowPosition;
    float4 pos : SV_Position;

};

Texture2D tex : register(t0);
Texture2D nmap : register(t2);

SamplerState splr : register(s0);


float4 main(PS_Input input) : SV_Target
{
    float3 diffuse;
    float3 specular;
    
    // shadow map test
    const float shadowLevel = Shadow(input.spos);
    if (shadowLevel != 0.0f)
    {
        // normalize the mesh normal
        input.viewNormal = normalize(input.viewNormal);
        // replace normal with mapped if normal mapping enabled
        if (useNormalMap)
        {
            const float3 mappedNormal = MapNormal(normalize(input.viewTan), normalize(input.viewBitan), input.viewNormal, input.tc, nmap, splr);
            input.viewNormal = lerp(input.viewNormal, mappedNormal, normalMapWeight);
        }
	    // fragment to light vector data
        const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewFragPos);
	    // attenuation
        const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	    // diffuse
        diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
        // specular
        specular = Speculate(
            diffuseColor * diffuseIntensity * specularColor, specularWeight, input.viewNormal,
            lv.vToL, input.viewFragPos, att, specularGloss
        );
        // scale by shadow level
        diffuse *= shadowLevel;
        specular *= shadowLevel;
    }
    else
    {
        diffuse = specular = 0.0f;
    }
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular), 1.0f);
}