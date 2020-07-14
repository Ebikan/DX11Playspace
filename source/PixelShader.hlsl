cbuffer cBuff {
	float4 faceColors[6];
};

float4 main( uint tid : SV_PrimitiveID ) : SV_TARGET
{
	
	return faceColors[tid / 2u]/ faceColors[tid / 2u].a;
}