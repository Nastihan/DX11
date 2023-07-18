#include "Transform.hlsli"
#include "VShadow.hlsli"

struct VS_Input
{
    float3 pos : POSITION;
    float3 n : NORMAL;
    float2 tc : TEXCOORD;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;

};

struct VS_Output
{
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
    float2 tc : TEXCOORD;
    float4 shadowHomoPos : ShadowPosition;
    float4 pos : SV_Position;
};

VS_Output main(VS_Input input)
{
    VS_Output output;
    output.viewPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    output.viewNormal = mul(input.n, (float3x3) modelView);
    output.tan = mul(input.tan, (float3x3) modelView);
    output.bitan = mul(input.bitan, (float3x3) modelView);
    output.pos = mul(float4(input.pos, 1.0f), modelViewProj);
    output.tc = input.tc;
    output.shadowHomoPos = ToShadowHomoSpace(input.pos, model);
    return output;
    
}