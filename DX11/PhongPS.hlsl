
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient = { 0.05f, 0.05f, 0.05f };
    float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
    float diffuseIntensity = 1.0f;
    float attConst = 0.5f;
    float attLin = 0.045f;
    float attQuad = 0.0075f;
};

struct PS_Input
{
    float3 worldPos : Position;
    float3 n : Normal;
    float4 pos : SV_Position;
};

static const float3 materialColor = { 0.7f, 0.2f, 0.1f };


float4 main(PS_Input input) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - input.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.n));
	// final color
    return float4(saturate((diffuse + ambient) * materialColor), 1.0f);
}