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
    float4 spos : ShadowPosition;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);

SamplerState splr : register(s0);

static const float specularPowerFactor = 100.0f;
float4 main(PS_Input input) : SV_Target
{
    float3 diffuse;
    float3 specularReflected;
    
    // sample diffuse texture
    const float4 dtex = tex.Sample(splr, input.tc);
    
#ifdef MASK_BOI
    // bail if highly translucent
    clip(dtex.a < 0.1f ? -1 : 1);
    // flip normal when backface
    if (dot(viewNormal, viewFragPos) >= 0.0f)
    {
        viewNormal = -viewNormal;
    }
#endif
    
    // shadow map test
    if (ShadowUnoccluded(input.spos))
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
        // specular parameter determination (mapped or uniform)
        float3 specularReflectionColor;
        float specularPower = specularGloss;
        const float4 specularSample = spec.Sample(splr, input.tc);
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
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
	    // attenuation
        const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	    // diffuse light
        diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
        // specular reflected
        specularReflected = Speculate(
            diffuseColor * diffuseIntensity * specularReflectionColor, specularWeight, input.viewNormal,
            lv.vToL, input.viewFragPos, att, specularPower
        );
    }
    else
    {
        diffuse = specularReflected = 0.0f;
    }
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4(saturate((diffuse + ambient) * dtex.rgb + specularReflected), 1.0f);
}