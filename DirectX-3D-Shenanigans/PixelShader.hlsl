//Best to use a structure to handle more than one attribute (e.g. position & color)
//Need to ensure this struct is the same as the equivalent vertex shader output
Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
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

float4 psmain(PS_INPUT input) : SV_TARGET
{
    
    //Light reflection models
    //PHONG REFLECTION MODEL
    //I_p = k_a*i_a + k_d(L_m dot N)i_m,d + (k_s(R_m dot V)^alpha)*i_ms 
    //k terms: magnitudes. i terms: colours
    //First term: reflections from environment
    //Second term: diffuse reflection, light scattered at many angles, reflected light hitting pixel
    //Third term: mirror-like reflections, reflection of angled light at pixel
    
    //Ambient reflection
    float ka = 0.1;
    //Ambient light colour; white = 1,1,1
    float3 ia = float3(1.0, 1.0, 1.0);
    float3 ambient_light = ka * ia;
    
    //diffuse reflection
    float kd = 1.0;
    float3 id = float3(1.0, 1.0, 1.0);
    //Want only positive values (set negative vals equivalent to 0 : no light)
    float amount_diffuse_light = max(0.0, dot(m_light_direction.xyz, input.normal));
    float3 diffuse_light = kd * amount_diffuse_light * id;
    
    //Specular light
    float ks = 1.0;
    float3 is = float3(1.0, 1.0, 1.0);
    //R Term: Use the Reflect model
    float3 reflected_light = reflect(m_light_direction.xyz, input.normal); //Will return the ray reflect at surface
    //alpha parameter: shininess, controls size of specular highlight
    float shininess = 10.0;
    float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);
    
    float3 specular_light = ks * amount_specular_light * is;
    
    
    //Sum ambient + diffuse + specular
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    //return float4(lerp(input.color, input.color1, (sin(m_angle) + 1.0f) / 2.0f), 1.0f);
   // return Texture.Sample(TextureSampler, input.texcoord*0.5);
    
    return float4(final_light, 1.0);

}