
cbuffer cb
{
    matrix modelView;
    matrix modelViewProj;
};

struct VS_Input
{
    float3 pos : POSITION;
    float3 n : NORMAL;
    float2 tc : TEXCOORD;
};

struct VS_Output
{
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;
};

VS_Output main( VS_Input input )  
{
    VS_Output output;
    output.viewPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    output.viewNormal = mul(input.n, (float3x3)modelView);
    output.pos = mul(float4(input.pos, 1.0f),modelViewProj);
    output.tc = input.tc;
    return output;
    
}