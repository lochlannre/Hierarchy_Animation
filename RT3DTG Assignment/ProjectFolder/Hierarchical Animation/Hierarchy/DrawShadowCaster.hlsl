cbuffer CommonApp
{
	float4x4 g_WVP;
}

struct VSInput
{
	float4 pos:POSITION;
};

struct PSInput
{
	float4 pos:SV_Position;
};

struct PSOutput
{
	float4 colour:SV_Target;
};

void VSMain(const VSInput input, out PSInput output)
{
	output.pos = mul(input.pos, g_WVP);
}

void PSMain(const PSInput input, out PSOutput output)
{
	output.colour = float4(0, 0, 1, 0.2);
}