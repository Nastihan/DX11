
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

struct PS_Input
{
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;
Texture2D nmap : register(t2);
SamplerState smplr;

float4 main(PS_Input input) : SV_Target
{
    if (normalMapEnabled)
    {
        const float3x3 tanTransform =   { 
            normalize(input.tan), normalize(input.bitan), normalize(input.viewNormal)
        };
        
        const float3 normalSample = nmap.Sample(smplr, input.tc).xyz;
        float3 tanNormal;
        tanNormal = normalSample * 2.0f - 1.0f;
        tanNormal.y = -tanNormal.y;
       
        
        input.viewNormal = normalize(mul(tanNormal, tanTransform));
    }

    // fragment to light vector data
    const float3 vToL = lightPos - input.viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.viewNormal));
	// reflected light vector
    const float3 w = input.viewNormal * dot(vToL, input.viewNormal);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(input.viewPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(smplr, input.tc).rgb + specular), 1.0f);
}