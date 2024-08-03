float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

float4 vsmain(float4 pos : POSITION) : SV_POSITION //SV_POSITION: says output of shader will contain final transformed vertex position in screen coords
{
    if (pos.y > 0 && pos.y < 1)
    {
        pos.x += 0.25f;
    }
    
    if (pos.y > -1 && pos.y < 0 && pos.x > 0 && pos.x < 1)
    {
        pos.y += 0.25f;
    }
    
    if (pos.y>0 && pos.y<1 && pos.x > -1 && pos.x < 0)
    {
        pos.y -= 0.25f;
    }
    
        return pos;
}