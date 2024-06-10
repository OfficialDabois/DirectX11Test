cbuffer PER_OBJECT {
    float4x4 WVP;
}

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUT VS(float4 inPos : POSITION, float4 inColor : COLOR)
{
    VS_OUT output;

    output.Pos = mul(inPos, WVP);
    output.Color = inColor;

    return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
    return input.Color;
}