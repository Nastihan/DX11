struct PS_Input
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

static const int kernelRadius = 13;
static const int divisor = (kernelRadius * 2 + 1) * (kernelRadius * 2 + 1);

float4 main(PS_Input input) : SV_TARGET
{        
    
    uint width, height;
    tex.GetDimensions(width,height);
    
    float pixelStepX = 1.0f / width;
    float pixelStepY = 1.0f / height;
    float accAlpha = 0.0f;
    float3 maxColor = float3(0.0f, 0.0f, 0.0f);    
    for (int y = -kernelRadius; y <= kernelRadius; y++)
    {
        for (int x = -kernelRadius; x <= kernelRadius; x++)
        {
            const float2 tc = input.uv + float2(pixelStepX * x, pixelStepY * y);
            const float4 s = tex.Sample(splr, tc).rgba;
            accAlpha += s.a;
            maxColor = max(s.rgb, maxColor);
        }
    }
    return float4(maxColor, accAlpha / divisor);
}