struct PS_Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

float4 main(PS_Input input) : SV_TARGET
{
    return 1.0f - tex.Sample(splr, input.uv).rgba;
}