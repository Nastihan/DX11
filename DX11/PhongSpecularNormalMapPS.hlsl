
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
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
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

Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);
SamplerState splr;

static const float specularPowerFactor = 100.0f;
float4 main(PS_Input input) : SV_Target
{
    if (normalMapEnabled)
    {
        const float3x3 tanTransform =
        {
            normalize(input.tan), normalize(input.bitan), normalize(input.viewNormal)
        };
        
        const float3 normalSample = nmap.Sample(splr, input.tc).xyz;
        float3 tanNormal;
        tanNormal = normalSample * 2.0f - 1.0f;
        // bring normal from tanspace into view space
        input.viewNormal = normalize(mul(tanNormal, tanTransform));
    }
    
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
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, input.tc);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(input.viewPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular * specularReflectionColor), 1.0f);
}