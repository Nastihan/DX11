
struct VS_Input
{
    float2 pos : POSITION;
};

struct VS_Output
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_Output main( VS_Input input ) 
{
    VS_Output output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = float2((input.pos.x + 1) / 2.0f, (1 - input.pos.y ) / 2.0f);
    return output;
}