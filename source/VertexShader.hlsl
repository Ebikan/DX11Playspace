struct VSOut {
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer {
	matrix transform : transform;
};


VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.f), transform);
	vso.color = color;
	return vso;
}