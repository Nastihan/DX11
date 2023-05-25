struct VS_Input
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct VS_Output
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD;
};

cbuffer cb
{
    matrix transformation;
};

VS_Output main( VS_Input input ) 
{
    VS_Output output;
    output.pos = mul(float4(input.pos, 1.0f),transformation);
    output.tex = input.tex;
    return output;
}