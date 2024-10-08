//Best to use a structure to handle more than one attribute (e.g. position & color)

struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 direction_to_camera : TEXCOORD2;
};


cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_light_direction;
    float4 m_camera_position;

};



VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	
//	output.position = lerp(input.position, input.position1, (float)((sin((float)(m_time / (float)1000.0f)) + 1.0f) / 2.0f));
	
	//WORLD SPACE
    output.position = mul(input.position, m_world);
    output.direction_to_camera = normalize(output.position.xyz - m_camera_position.xyz); //use normalise to get a vector direction
	//VIEW SPACE
    output.position = mul(output.position, m_view);
	//SCREEN SPACE
    output.position = mul(output.position, m_proj);
    
    output.normal = input.normal;


    output.texcoord = input.texcoord;
    output.normal = input.normal;
    return output;
}