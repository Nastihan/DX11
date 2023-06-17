#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"



cbuffer ObjectCBuf
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};
struct PS_Input
{
    float3 viewFragPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTan : TANGENT;
    float3 viewBitan : BITANGENT;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);
SamplerState splr;

static const float specularPowerFactor = 100.0f;
float4 main(PS_Input input) : SV_Target
{
    input.viewNormal = normalize(input.viewNormal);
    if (normalMapEnabled)
    {
        input.viewNormal = MapNormal(normalize(input.viewTan), normalize(input.viewBitan), input.viewNormal, input.tc, nmap, splr);
    }
    
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewFragPos);

    // specular parameter determination (mapped or uniform)
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, input.tc);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL , input.viewNormal);
    // specular reflected
    const float3 specularReflected = Speculate(
        specularReflectionColor, 1.0f, input.viewNormal,
        lv.vToL, input.viewFragPos, att, specularPower
    ); // final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specularReflected), 1.0f);
}