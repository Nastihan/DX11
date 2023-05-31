
cbuffer cb
{
    matrix model;
    matrix modelViewProj;
};

struct VS_Input
{
    float3 pos : POSITION;
    float3 n : NORMAL;
};

struct VS_Output
{
    float3 worldPos : Position;
    float3 n : Normal;
    float4 pos : SV_Position;
};

VS_Output main( VS_Input input )  
{
    VS_Output output;
    output.worldPos = mul(float4(input.pos, 1.0f), model);
    output.n = mul(input.n, (float3x3)model);
    output.pos = mul(float4(input.pos, 1.0f),modelViewProj);
    return output;
    
}