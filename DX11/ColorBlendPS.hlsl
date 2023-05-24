struct PS_Input
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};


float4 main(PS_Input PS_In) : SV_TARGET
{
	return PS_In.color;
}