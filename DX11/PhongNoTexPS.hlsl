
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
    float4 materialColor;
    float4 specularColor;
    float specularPower;
};

struct PS_Input
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float4 pos : SV_Position;
};


float4 main(PS_Input input) : SV_Target
{
    input.viewNormal = normalize(input.viewNormal);
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
    const float4 specular = att * (float4(diffuseColor, 1.0f) * diffuseIntensity) * specularColor * pow(max(0.0f, dot(normalize(-r), normalize(input.viewPos))), specularPower);
	// final color
    return saturate(float4(diffuse + ambient, 1.0f) * materialColor + specular);
}