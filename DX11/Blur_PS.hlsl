struct PS_Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

static const int kernelRadius = 3;

float4 main(PS_Input input) : SV_TARGET
{
    uint width, height;
    tex.GetDimensions(width,height);
    
    float pixelStepX = 1.0f / width;
    float pixelStepY = 1.0f / height;
    
   
    float4 acc = (0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int y = -kernelRadius; y <= kernelRadius; y++)
    {
        for (int x = -kernelRadius; x <= kernelRadius; x++)
        {
            float2 tc = float2 (input.uv.x + (pixelStepX * x), input.uv.y + (pixelStepY * y));
            float4 result = tex.Sample(splr, tc).rgba;
            
            acc += result;
        }
    }
    
    return acc / 49;
    
}