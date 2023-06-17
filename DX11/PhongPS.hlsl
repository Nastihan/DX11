
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer materialColorCBuf
{
    float specularIntensity;
    float specularPower;
    float2 padding;
};

struct PS_Input
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;
SamplerState smplr;

float4 main(PS_Input input) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - input.viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.viewNormal));
    // reflected light vector
    const float3 w = input.viewNormal * dot(vToL, input.viewNormal);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(input.viewPos))), specularPower);
	// final color
    return float4(saturate(diffuse + ambient) * tex.Sample(smplr, input.tc).rgb + specular, 1.0f);
}