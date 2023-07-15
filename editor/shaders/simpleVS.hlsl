// Simple Sample Shader to show the basics of HLSL with vulkan
// References:
// https://github.com/KhronosGroup/Vulkan-Guide/blob/main/chapters/hlsl.adoc
// https://anteru.net/blog/2016/mapping-between-HLSL-and-GLSL/
// https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/SPIR-V.rst#the-implicit-vk-namespace)
// https://github.com/microsoft/DirectXShaderCompiler/wiki/Using-dxc.exe-and-dxcompiler.dll

// Vertex Shader

struct VSInput
{
    uint VertexIndex : SV_VertexID;
};

struct VSOutput
{
    float4 Position : SV_Position;
};


VSOutput main(VSInput input)
{
    float2 position[3];

    position[0] = float2(0.0, -0.5);
    position[1] = float2(0.5, 0.5);
    position[2] = float2(-0.5, 0.5);

    VSOutput output;
    output.Position = float4(position[input.VertexIndex], 0.0, 1.0);
    return output;
}
