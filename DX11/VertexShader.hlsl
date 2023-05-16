struct VS_Input
{
    float2 pos : POSITION;
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
    output.pos = mul(float4(input.pos.x, input.pos.y, 0.0f, 1.0f), transformation);
    output.color = input.color;
    return output;
    
}