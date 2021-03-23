//////////////////////////////////////////////////////////////////////
// HLSL File:
// This example is compiled using the fxc shader compiler.
// It is possible directly compile HLSL in VS2013
//////////////////////////////////////////////////////////////////////

// This first constant buffer is special.
// The framework looks for particular variables and sets them automatically.
// See the CommonApp comments for the names it looks for.
#define MAX_NUM_LIGHTS 4
cbuffer CommonApp
{
	float4x4 g_WVP;
	float4 g_lightDirections[MAX_NUM_LIGHTS];
	float4 g_lightPositions[MAX_NUM_LIGHTS];
	float3 g_lightColours[MAX_NUM_LIGHTS];
	float4 g_lightAttenuations[MAX_NUM_LIGHTS];
	float4 g_lightSpots[MAX_NUM_LIGHTS];
	int g_numLights;
	float4x4 g_InvXposeW;
	float4x4 g_W;
};


// When you define your own cbuffer you can use a matching structure in your app but you must be careful to match data alignment.
// Alternatively, you may use shader reflection to find offsets into buffers based on variable names.
// The compiler may optimise away the entire cbuffer if it is not used but it shouldn't remove indivdual variables within it.
// Any 'global' variables that are outside an explicit cbuffer go
// into a special cbuffer called "$Globals". This is more difficult to work with
// because you must use reflection to find them.
// Also, the compiler may optimise individual globals away if they are not used.
cbuffer MyApp
{
	float	g_frameCount;
	float3	g_waveOrigin;
}

//Helper functions
float4 GetLightingColour(float3 worldPos, float3 N)
{
	float4 lightingColour = float4(0, 0, 0, 1);
	for (int i = 0; i < g_numLights; ++i)
	{
		float3 D = g_lightPositions[i].w * (g_lightPositions[i].xyz - worldPos);
		float dotDD = dot(D, D);
		if (dotDD > g_lightAttenuations[i].w)
			continue;
		float atten = 1.0 / (g_lightAttenuations[i].x + g_lightAttenuations[i].y * length(D) + g_lightAttenuations[i].z * dot(D, D));

		float3 L = g_lightDirections[i].xyz;
		float dotNL = g_lightDirections[i].w * saturate(dot(N, L));

		float rho = 0.0;
		if (dotDD > 0.0)
			rho = dot(L, normalize(D));

		float spot;
		if (rho > g_lightSpots[i].y)
			rho = 1.0;
		else if (rho < g_lightSpots[i].x)
			spot = 0.0;
		else
			spot = pow((rho - g_lightSpots[i].x) * g_lightSpots[i].z, g_lightSpots[i].w);

		float3 light = atten * spot * g_lightColours[i];
		if (g_lightDirections[i].w > 0.f)
			light *= dotNL;
		else
			light *= saturate(dot(N, normalize(D)));

		lightingColour.xyz += light;
	}
	return lightingColour;

}

cbuffer DrawHeightMap
{
	float4x4 g_shadowViewMatrix;
	float4 g_shadowColour;
};

Texture2D g_shadowTexture;
SamplerState g_shadowSampler;


// VSInput structure defines the vertex format expected by the input assembler when this shader is bound.
// You can find a matching structure in the C++ code.
struct VSInput
{
	float4 pos:POSITION;
	float4 colour:COLOUR0;
	float3 normal:NORMAL;
	float2 tex:TEXCOORD;
};

// PSInput structure is defining the output of the vertex shader and the input of the pixel shader.
// The variables are interpolated smoothly across triangles by the rasteriser.
struct PSInput
{
	float4 pos:SV_Position;
	float4 colour:COLOUR0;
	float4 w_pos:POSITION;
	float3 normal:NORMAL;
	float2 tex:TEXCOORD;
	float4 mat:COLOUR1;
	//float4 light:COLOUR2;
};

// PSOutput structure is defining the output of the pixel shader, just a colour value.
struct PSOutput
{
	float4 colour:SV_Target;
};

// Define several Texture 'slots'
Texture2D g_materialMap;
Texture2D g_texture0;
Texture2D g_texture1;
Texture2D g_texture2;


// Define a state setting 'slot' for the sampler e.g. wrap/clamp modes, filtering etc.
SamplerState g_sampler;



// The vertex shader entry point. This function takes a single vertex and transforms it for the rasteriser.
void VSMain(const VSInput input, out PSInput output)
{
	//Work out position of vertex on the material map

	//Vertexs run from -512 -> 512, Material map is 0->512 so must be converted.
	float2 pos = { input.pos.x, input.pos.z };
	//Convert into 0 -> 1024
	pos += 512;
	//Convert to 0 -> 512
	pos /= 2;
	//Sample Level function expects the pos value to be between 0.0 -> 1.0 so normalize
	pos /= 512;

	//DDS has been flipped to fix data
	//To do this without touching the orignal file you would do
	//pos = (pos * -1) + 1;

	output.colour.rgb = g_materialMap.SampleLevel(g_sampler, pos, 0);
	output.colour.a = 1;


	output.pos = mul(input.pos, g_WVP);
	output.normal = input.normal;
	output.tex = input.tex;

    float3 worldNormal = mul(input.normal, g_InvXposeW);
	//output.light = input.colour * GetLightingColour(input.pos, normalize(worldNormal));
	output.w_pos = input.pos;
}

// The pixel shader entry point. This function writes out the fragment/pixel colour.
void PSMain(const PSInput input, out PSOutput output)
{
	float3 normal = input.normal;

	float4 materialMap = input.colour;
	float4 colour = { 0,0,0,1 };

	colour = lerp(colour, g_texture0.Sample(g_sampler, input.tex), materialMap.r);
	colour = lerp(colour, g_texture1.Sample(g_sampler, input.tex), materialMap.g);
	colour = lerp(colour, g_texture2.Sample(g_sampler, input.tex), materialMap.b);

	float3 light = { 0.5,0.5,0.5 };
	for (int i = 0; i < g_numLights; ++i)
	{
		//light = lerp(light, g_lightColours[i], dot(g_lightDirections[i], normal));
		light += dot(normal, g_lightDirections[i]) * g_lightColours[i];
	}

	colour.rgb *= light;

	//Transform Pixel into world space
	float4 pos = mul(input.w_pos, g_shadowViewMatrix);

	//Perspective Correction
	//pos.xy /= pos.w;

	//Change range to 0->1
	//pos.xy += 1;
	//pos.xy /= 2;

	//pos.y *= -1;
	//pos.y += 1;


	//Sample shadow texture
	float4 col_sample = g_shadowTexture.Sample(g_shadowSampler, float2( 0.5f, 0.5f ));
	//Check if in shadow and 'infront' of light
	bool isShadow = (col_sample.rgb == float3(0, 0, 1) && pos.w >= 0);


	if (isShadow)
	{
		//Lerp between terrain color and shadow colour
		colour = lerp(colour, g_shadowColour, col_sample.a);
	}

	output.colour = colour;	// 'return' the colour value for this fragment.

	//.output.colour.a = 1;
}