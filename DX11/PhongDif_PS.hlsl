#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"

#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

struct PS_input
{
    float3 viewFragPos : POSITION;
    float3 viewNormal : NORMAL;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;

SamplerState splr;


float4 main(PS_input input) : SV_Target
{
    // renormalize interpolated normal
    input.viewNormal = normalize(input.viewNormal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewFragPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
	// specular
    const float3 specular = Speculate(diffuseColor * diffuseIntensity * specularColor, specularWeight, input.viewNormal, lv.vToL, input.viewFragPos, att, specularGloss);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular), 1.0f);
}