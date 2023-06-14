
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
    bool normalMapEnabled;
    float padding[3];
};

struct PS_Input
{
    float3 worldPos : POSITION;
    float3 n : NORMAL;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;
Texture2D spec;
Texture2D nmap;
SamplerState splr;

static const float specularPowerFactor = 100.0f;
float4 main(PS_Input input) : SV_Target
{
    if (normalMapEnabled)
    {
        const float3x3 tanTransform =
        {
            normalize(input.tan), normalize(input.bitan), normalize(input.n)
        };
        
        const float3 normalSample = nmap.Sample(splr, input.tc).xyz;
        input.n = normalSample * 2.0f - 1.0f;
        input.n.y = -input.n.y;
        input.n.z = -input.n.z;
        
        input.n = mul(input.n, tanTransform);
    }
    
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
    const float specularPower = pow(2.0f, specularSample.a * 15.0f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(input.worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular * specularReflectionColor), 1.0f);
}