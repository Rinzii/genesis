struct VertexInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};


PixelInput VertexShader(VertexInput input)
{
	PixelInput output;
	output.position = mul(float4(input.position, 1.0), worldViewProj);
	output.normal = normalize(input.normal);
	return output;
}

float4 PixelShader(PixelInput input) : SV_Target
{
	float3 lightDir = normalize(lightDirection);
	float3 normal = normalize(input.normal);
    
	// Calculate the diffuse factor
	float diffuseFactor = saturate(dot(normal, lightDir));
    
	// Calculate the ambient factor using the hemisphere technique
	float3 ambientUp = float3(0.5, 0.5, 1.0);  // Color of the upper hemisphere
	float3 ambientDown = float3(0.1, 0.1, 0.2);  // Color of the lower hemisphere
    
	float3 ambientFactor = lerp(ambientDown, ambientUp, 0.5 * (normal.y + 1.0));
    
	// Combine the diffuse and ambient factors to get the final color
	float3 finalColor = diffuseFactor * lightColor.xyz + ambientFactor;
    
	return float4(finalColor, 1.0);
}

VertexInput DefaultVertexShader(VertexInput input)
{
	return input;
}

float4 DefaultPixelShader(PixelInput input) : SV_Target
{
	return float4(1.0, 1.0, 1.0, 1.0); // Default pixel shader (returns white)
}

void main(in VertexInput input, out PixelInput output)
{
	output = VertexShader(input);
}

float4 main(PixelInput input) : SV_Target
{
	return PixelShader(input);
}
