//Best to use a structure to handle more than one attribute (e.g. position & color)
//Need to ensure this struct is the same as the equivalent vertex shader output
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};


//float4 main() : SV_TARGET
//{
//    
//	return float4(1.0f, 1.0f, 1.0f, 1.0f); //returns white colour
//}


float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(input.color, 1.0f);

}