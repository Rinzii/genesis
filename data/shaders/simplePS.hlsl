// Simple Sample Shader to show the basics of HLSL with vulkan
// References:
// https://github.com/KhronosGroup/Vulkan-Guide/blob/main/chapters/hlsl.adoc
// https://anteru.net/blog/2016/mapping-between-HLSL-and-GLSL/
// https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/SPIR-V.rst#the-implicit-vk-namespace)
// https://github.com/microsoft/DirectXShaderCompiler/wiki/Using-dxc.exe-and-dxcompiler.dll


// Pixel Shader (Fragment Shader)

struct PSOutput
{
    float4 outColor : SV_Target0;
};

PSOutput main()
{
    PSOutput output;
    output.outColor = float4(1.0, 0.0, 0.0, 1.0);
    return output;
}
