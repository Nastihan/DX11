#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    bool useGlossAlpha;
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
    float4 dtex = tex.Sample(splr, input.tc);

#ifdef MASK
    // alpha testing
    float alpha = tex.Sample(splr, input.tc).a;
    clip(alpha > 0.1 ? 1.0 : -1.0);
    // flip normal when backface
    if (dot(input.viewNormal, input.viewFragPos) >= 0.0f)
    {
        input.viewNormal = -input.viewNormal;
    }
#endif
    
    input.viewNormal = normalize(input.viewNormal);
    if (useNormalMap)
    {
        input.viewNormal = MapNormal(normalize(input.viewTan), normalize(input.viewBitan), input.viewNormal, input.tc, nmap, splr);
    }
    
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewFragPos);

    // specular parameter determination (mapped or uniform)
    float3 specularReflectionColor;
    float specularPower = specularGloss;
    const float4 specularSample = spec.Sample(splr, input.tc);
    specularReflectionColor = specularSample.rgb;
    
    if (useGlossAlpha)
    {
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
    
    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
    // specular reflected
    const float3 specularReflected = Speculate(
      diffuseColor * diffuseIntensity * specularReflectionColor, specularWeight, input.viewNormal,
        lv.vToL, input.viewFragPos, att, specularPower
    );
    // final color
    return float4(saturate((diffuse + ambient) * dtex.rgb + specularReflected), 1.0f);
}