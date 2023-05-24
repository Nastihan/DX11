
cbuffer cb
{
    matrix transform;
};

struct VS_Input
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_Output
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VS_Output main( VS_Input VS_In ) 
{
    VS_Output VS_Out;
    VS_Out.pos = mul(float4(VS_In.pos, 1.0f), transform);
    VS_Out.color = VS_In.color;
    return VS_Out;
}