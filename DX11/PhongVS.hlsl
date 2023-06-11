
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
    float3 worldPos : POSITION;
    float3 n : NORMAL;
    float4 pos : SV_Position;
    float2 tc : TEXCOORD;

};

VS_Output main( VS_Input input )  
{
    VS_Output output;
    output.worldPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    output.n = mul(input.n, (float3x3)modelView);
    output.pos = mul(float4(input.pos, 1.0f),modelViewProj);
    output.tc = input.tc;
    return output;
    
}