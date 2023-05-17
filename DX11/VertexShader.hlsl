struct VS_Input
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer cb
{
    matrix transformation;
};

VS_Output main(VS_Input input) 
{
    VS_Output output;
    output.pos = mul(float4(input.pos, 1.0f), transformation);
    output.color = input.color;
    return output;
    
}