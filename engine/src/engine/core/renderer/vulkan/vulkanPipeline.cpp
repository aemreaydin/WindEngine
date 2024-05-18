#include "vulkanPipeline.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include <glm/vec3.hpp>

namespace WindEngine::Core::Render
{

VulkanPipeline::VulkanPipeline( VulkanDevice& device, vk::AllocationCallbacks* allocator )
  : VulkanHandle( &device, allocator )
{
}

void VulkanPipeline::Initialize( const vk::RenderPass& renderPass )
{
    InitializeShaderStage( "shaders/simple_vert.spv", "shaders/simple_frag.spv" );
    InitializeVertexInputState();
    InitializeInputAssemblyState();
    InitializeViewportState();
    InitializeRasterizationState();
    InitializeMultisampleState();
    InitializeDepthStencilState();
    InitializeColorBlendState();
    InitializeDynamicState();

    const auto layoutInfo = vk::PipelineLayoutCreateInfo {};
    _pipelineLayout = _device->device.createPipelineLayout( layoutInfo, _allocator );

    const auto pipelineInfo = vk::GraphicsPipelineCreateInfo {
        .stageCount = ToU32( _shaderInfos.size() ),
        .pStages = _shaderInfos.data(),
        .pVertexInputState = &_vertexInputInfo,
        .pInputAssemblyState = &_inputAssemblyInfo,
        .pTessellationState = nullptr,  // TODO(emreaydn): Add functionality for tesselation
        .pViewportState = &_viewportInfo,
        .pRasterizationState = &_rasterizationInfo,
        .pMultisampleState = &_multisampleInfo,
        .pDepthStencilState = &_depthStencilInfo,
        .pColorBlendState = &_colorBlendInfo,
        .pDynamicState = &_dynamicInfo,
        .layout = _pipelineLayout,
        .renderPass = renderPass,
        .subpass = 0,
    };
    auto [result, graphicsPipeline] = _device->device.createGraphicsPipeline( nullptr, pipelineInfo, _allocator );
    if ( result != vk::Result::eSuccess )
    {
        WindFatal( "Failed to create the pipeline." );
    }
    _pipeline = graphicsPipeline;
}

void VulkanPipeline::Destroy()
{
    for ( const auto& module : _shaderModules )
    {
        _device->device.destroy( module );
    }
    _device->device.destroy( _pipelineLayout, _allocator );
    _device->device.destroy( _pipeline, _allocator );
}

void VulkanPipeline::InitializeShaderStage( const std::string& vertFile, const std::string& fragFile )

{
    _shaderModules = { CreateShaderModule( vertFile ), CreateShaderModule( fragFile ) };
    const auto vertInfo = vk::PipelineShaderStageCreateInfo { .stage = vk::ShaderStageFlagBits::eVertex,
                                                              .module = _shaderModules.at( 0 ),
                                                              .pName = "main",
                                                              .pSpecializationInfo = nullptr };
    const auto fragInfo = vk::PipelineShaderStageCreateInfo { .stage = vk::ShaderStageFlagBits::eFragment,
                                                              .module = _shaderModules.at( 1 ),
                                                              .pName = "main",
                                                              .pSpecializationInfo = nullptr };
    _shaderInfos = { vertInfo, fragInfo };
}

void VulkanPipeline::InitializeVertexInputState()
{
    static const auto bindingDesc = vk::VertexInputBindingDescription { .binding = 0,
                                                                        .stride = sizeof( glm::vec3 ) * 2,
                                                                        .inputRate = vk::VertexInputRate::eVertex };
    static const std::vector<vk::VertexInputAttributeDescription> attributeDesc {
        { .location = 0, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = 0 },
        { .location = 1, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = sizeof( glm::vec3 ) }
    };

    _vertexInputInfo = { .vertexBindingDescriptionCount = 1,
                         .pVertexBindingDescriptions = &bindingDesc,
                         .vertexAttributeDescriptionCount = ToU32( attributeDesc.size() ),
                         .pVertexAttributeDescriptions = attributeDesc.data() };
}

void VulkanPipeline::InitializeInputAssemblyState()
{
    _inputAssemblyInfo = { .topology = vk::PrimitiveTopology::eTriangleList, .primitiveRestartEnable = VK_FALSE };
}

void VulkanPipeline::InitializeViewportState()
{
    _viewportInfo = { .viewportCount = 1, .pViewports = nullptr, .scissorCount = 1, .pScissors = nullptr };
}

void VulkanPipeline::InitializeRasterizationState()
{
    _rasterizationInfo = { .depthClampEnable = VK_FALSE,
                           .rasterizerDiscardEnable = VK_FALSE,
                           .polygonMode = vk::PolygonMode::eFill,
                           .cullMode = vk::CullModeFlagBits::eBack,
                           .frontFace = vk::FrontFace::eCounterClockwise,
                           .depthBiasEnable = VK_FALSE,
                           .depthBiasConstantFactor = 0.0F,
                           .depthBiasClamp = 0.0F,
                           .depthBiasSlopeFactor = 0.0F,
                           .lineWidth = 1.0F };
}

void VulkanPipeline::InitializeMultisampleState()
{
    _multisampleInfo = { .rasterizationSamples = vk::SampleCountFlagBits::e1,
                         .sampleShadingEnable = VK_FALSE,
                         .minSampleShading = 0.0F,
                         .pSampleMask = nullptr,
                         .alphaToCoverageEnable = VK_FALSE,
                         .alphaToOneEnable = VK_FALSE };
}

void VulkanPipeline::InitializeDepthStencilState()
{
    _depthStencilInfo = {
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = vk::CompareOp::eLessOrEqual,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        .front = {},
        .back = {},
        .minDepthBounds = 0.0F,
        .maxDepthBounds = 1.0F,
    };
}

void VulkanPipeline::InitializeColorBlendState()
{
    static const auto colorAttachment =
      vk::PipelineColorBlendAttachmentState { .blendEnable = VK_FALSE,
                                              .srcColorBlendFactor = vk::BlendFactor::eOne,
                                              .dstColorBlendFactor = vk::BlendFactor::eOne,
                                              .colorBlendOp = vk::BlendOp::eAdd,
                                              .srcAlphaBlendFactor = vk::BlendFactor::eOne,
                                              .dstAlphaBlendFactor = vk::BlendFactor::eOne,
                                              .alphaBlendOp = vk::BlendOp::eMin,
                                              .colorWriteMask =
                                                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };
    const auto blendConstants = std::array<float, 4> { 0.0F, 0.0F, 0.0F, 0.0F };  // TODO(emreaydn): Try this
    _colorBlendInfo = { .logicOpEnable = VK_FALSE,
                        .logicOp = vk::LogicOp::eCopy,
                        .attachmentCount = 1,
                        .pAttachments = &colorAttachment,
                        .blendConstants = blendConstants };
}

void VulkanPipeline::InitializeDynamicState()
{
    static const auto dynamicStates =
      std::vector<vk::DynamicState> { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    _dynamicInfo = { .dynamicStateCount = ToU32( dynamicStates.size() ), .pDynamicStates = dynamicStates.data() };
}

auto VulkanPipeline::CreateShaderModule( const std::string& file ) -> vk::ShaderModule
{
    const auto shaderCode = G_READ_SHADER_FROM_FILE<U32>( file );
    const auto shaderInfo = vk::ShaderModuleCreateInfo { .codeSize = shaderCode.size(), .pCode = shaderCode.data() };
    return _device->device.createShaderModule( shaderInfo, _allocator );
}

auto VulkanPipeline::GetPipeline() const -> const vk::Pipeline&
{
    return _pipeline;
}

auto VulkanPipeline::GetPipelineLayout() const -> const vk::PipelineLayout&
{
    return _pipelineLayout;
}

}  // namespace WindEngine::Core::Render
