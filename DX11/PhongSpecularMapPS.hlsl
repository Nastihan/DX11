
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


struct PS_Input
{
    float3 worldPos : Position;
    float3 n : Normal;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;
Texture2D spec;
SamplerState splr;

static const float specularPowerFactor = 122.0f;
float4 main(PS_Input input) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - input.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.n));
    // reflected light vector
    const float3 w = input.n * dot(vToL, input.n);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float4 specularSample = spec.Sample(splr, input.tc);
    const float3 specularReflectionColor = specularSample.rgb;
    const float specularPower = pow(2.0f, specularSample.a * 13.0f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(input.worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular * specularReflectionColor), 1.0f);
}