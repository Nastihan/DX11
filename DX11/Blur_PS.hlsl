struct PS_Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

static const int kernelRadius = 5;
static const int divisor = (kernelRadius * 2 + 1) * (kernelRadius * 2 + 1);

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
            const float2 tc = input.uv + float2(pixelStepX * x, pixelStepY * y);
            float4 result = tex.Sample(splr, tc).rgba;
            
            acc += result;
        }
    }
    
    return acc / divisor;
    
}