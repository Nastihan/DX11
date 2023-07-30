#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"

#include "PShadow.hlsli"

cbuffer ObjectCBuf : register(b1)
{
    bool useGlossAlpha;
    bool useSpecularMap;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

struct PS_Input
{
    float3 viewFragPos : POSITION;
    float3 viewNormal : NORMAL;
    float2 tc : TEXCOORD;
    float4 spos : ShadowPosition;
    float4 pos : SV_Position;
};
Texture2D tex : register(t0);
Texture2D spec : register(t1);

SamplerState splr : register(s0);


float4 main(PS_Input input) : SV_Target
{
    float3 diffuse;
    float3 specularReflected;
    
    // shadow map test
    const float shadowLevel = Shadow(input.spos);
    if (shadowLevel != 0.0f)
    {
        // normalize the mesh normal
        input.viewNormal = normalize(input.viewNormal);
	    // fragment to light vector data
        const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewFragPos);
        // specular parameters
        float specularPowerLoaded = specularGloss;
        const float4 specularSample = spec.Sample(splr, input.tc);
        float3 specularReflectionColor;
        if (useSpecularMap)
        {
            specularReflectionColor = specularSample.rgb;
        }
        else
        {
            specularReflectionColor = specularColor;
        }
        if (useGlossAlpha)
        {
            specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
        }
	    // attenuation
        const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	    // diffuse light
        diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL,input.viewNormal);
        // specular reflected
        specularReflected = Speculate(
            diffuseColor * specularReflectionColor, specularWeight, input.viewNormal,
            lv.vToL, input.viewFragPos, att, specularPowerLoaded
        );
        // scale by shadow level
        diffuse *= shadowLevel;
    }
    else
    {
        diffuse = specularReflected = 0.0f;
    }
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specularReflected), 1.0f);
}