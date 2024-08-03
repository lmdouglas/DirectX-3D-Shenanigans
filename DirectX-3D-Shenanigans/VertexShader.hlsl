//Best to use a structure to handle more than one attribute (e.g. position & color)
struct VS_INPUT
{
    float4 position : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};


VS_OUTPUT vsmain(VS_INPUT input) //SV_POSITION: says output of shader will contain final transformed vertex position in screen coords
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    output.position = input.position;
    output.color = input.color;
    
    return output;
}