#ifndef WINDENGINE_VULKANPIPELINE_HPP
#define WINDENGINE_VULKANPIPELINE_HPP

#include "vulkanHandle.hpp"

namespace WindEngine::Core::Render
{

struct VulkanPipeline : public VulkanHandle
{
    VulkanPipeline( VulkanDevice& device, vk::AllocationCallbacks* allocator );

    void Initialize( const vk::RenderPass& renderPass );
    void Destroy() override;

    [[nodiscard]] auto GetPipeline() const -> const vk::Pipeline&;

    [[nodiscard]] auto GetPipelineLayout() const -> const vk::PipelineLayout&;

private:
    void InitializeShaderStage( const std::string& vertFile, const std::string& fragFile );
    void InitializeVertexInputState();
    void InitializeInputAssemblyState();
    void InitializeViewportState();
    void InitializeRasterizationState();
    void InitializeMultisampleState();
    void InitializeDepthStencilState();
    void InitializeColorBlendState();
    void InitializeDynamicState();

    [[nodiscard]] auto CreateShaderModule( const std::string& file ) -> vk::ShaderModule;

    vk::PipelineLayout _pipelineLayout;
    vk::Pipeline _pipeline;
    std::vector<vk::ShaderModule> _shaderModules;
    std::vector<vk::PipelineShaderStageCreateInfo> _shaderInfos;
    vk::PipelineVertexInputStateCreateInfo _vertexInputInfo {};
    vk::PipelineInputAssemblyStateCreateInfo _inputAssemblyInfo {};
    vk::PipelineViewportStateCreateInfo _viewportInfo {};
    vk::PipelineRasterizationStateCreateInfo _rasterizationInfo {};
    vk::PipelineMultisampleStateCreateInfo _multisampleInfo {};
    vk::PipelineDepthStencilStateCreateInfo _depthStencilInfo {};
    vk::PipelineColorBlendStateCreateInfo _colorBlendInfo {};
    vk::PipelineDynamicStateCreateInfo _dynamicInfo {};
};

}  // namespace WindEngine::Core::Render

#endif  // WINDENGINE_VULKANPIPELINE_HPP
