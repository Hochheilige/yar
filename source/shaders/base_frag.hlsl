struct PSInput {
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 tex_coord : TEXCOORD;
};

Texture2D<float4> logo : register(t0, space0);
SamplerState samplerState : register(s0, space0);

cbuffer c_color : register(b0, space1)
{
    float3 rgb;
    float padding;
};

float4 main(PSInput input) : SV_TARGET {
    float4 textureColor = logo.Sample(samplerState, input.tex_coord);
    input.color += rgb;
    return float4(textureColor.rgb * input.color, 1.0f);
}