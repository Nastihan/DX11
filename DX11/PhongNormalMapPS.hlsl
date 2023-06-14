
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

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

cbuffer transform
{
    matrix modelView;
    matrix modelViewProj;
}; 

struct PS_Input
{
    float3 worldPos : Position;
    float3 n : Normal;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;
Texture2D nmap;
SamplerState smplr;

float4 main(PS_Input input) : SV_Target
{
    if (normalMapEnabled)
    {
        const float3 normalSample = nmap.Sample(smplr, input.tc).xyz;
        input.n = normalSample * 2.0f - 1.0f;
        input.n.y = -input.n.y;
        input.n.z = -input.n.z;
        
        input.n = mul(input.n, modelView);
    }

    // fragment to light vector data
    const float3 vToL = lightPos - input.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.n));
	// reflected light vector
    const float3 w = input.n * dot(vToL, input.n);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(input.worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(smplr, input.tc).rgb + specular), 1.0f);
}