
Texture2D uvs;

SamplerState samplerstate;

struct PS_Input
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD;
};

float4 main(PS_Input input) : SV_TARGET
{
    
    return uvs.Sample(samplerstate, input.tex);
}