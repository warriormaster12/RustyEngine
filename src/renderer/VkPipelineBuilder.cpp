#include "VkTools.h"
#include "VkInit.h"

#include "third-party/spirv-reflect/spirv_reflect.h"

#include <cstddef>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <array>


static uint32_t FormatSize(VkFormat format)
{
  uint32_t result = 0;
  switch (format) {
  case VK_FORMAT_UNDEFINED: result = 0; break;
  case VK_FORMAT_R4G4_UNORM_PACK8: result = 1; break;
  case VK_FORMAT_R4G4B4A4_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_B4G4R4A4_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_R5G6B5_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_B5G6R5_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_R5G5B5A1_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_B5G5R5A1_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_A1R5G5B5_UNORM_PACK16: result = 2; break;
  case VK_FORMAT_R8_UNORM: result = 1; break;
  case VK_FORMAT_R8_SNORM: result = 1; break;
  case VK_FORMAT_R8_USCALED: result = 1; break;
  case VK_FORMAT_R8_SSCALED: result = 1; break;
  case VK_FORMAT_R8_UINT: result = 1; break;
  case VK_FORMAT_R8_SINT: result = 1; break;
  case VK_FORMAT_R8_SRGB: result = 1; break;
  case VK_FORMAT_R8G8_UNORM: result = 2; break;
  case VK_FORMAT_R8G8_SNORM: result = 2; break;
  case VK_FORMAT_R8G8_USCALED: result = 2; break;
  case VK_FORMAT_R8G8_SSCALED: result = 2; break;
  case VK_FORMAT_R8G8_UINT: result = 2; break;
  case VK_FORMAT_R8G8_SINT: result = 2; break;
  case VK_FORMAT_R8G8_SRGB: result = 2; break;
  case VK_FORMAT_R8G8B8_UNORM: result = 3; break;
  case VK_FORMAT_R8G8B8_SNORM: result = 3; break;
  case VK_FORMAT_R8G8B8_USCALED: result = 3; break;
  case VK_FORMAT_R8G8B8_SSCALED: result = 3; break;
  case VK_FORMAT_R8G8B8_UINT: result = 3; break;
  case VK_FORMAT_R8G8B8_SINT: result = 3; break;
  case VK_FORMAT_R8G8B8_SRGB: result = 3; break;
  case VK_FORMAT_B8G8R8_UNORM: result = 3; break;
  case VK_FORMAT_B8G8R8_SNORM: result = 3; break;
  case VK_FORMAT_B8G8R8_USCALED: result = 3; break;
  case VK_FORMAT_B8G8R8_SSCALED: result = 3; break;
  case VK_FORMAT_B8G8R8_UINT: result = 3; break;
  case VK_FORMAT_B8G8R8_SINT: result = 3; break;
  case VK_FORMAT_B8G8R8_SRGB: result = 3; break;
  case VK_FORMAT_R8G8B8A8_UNORM: result = 4; break;
  case VK_FORMAT_R8G8B8A8_SNORM: result = 4; break;
  case VK_FORMAT_R8G8B8A8_USCALED: result = 4; break;
  case VK_FORMAT_R8G8B8A8_SSCALED: result = 4; break;
  case VK_FORMAT_R8G8B8A8_UINT: result = 4; break;
  case VK_FORMAT_R8G8B8A8_SINT: result = 4; break;
  case VK_FORMAT_R8G8B8A8_SRGB: result = 4; break;
  case VK_FORMAT_B8G8R8A8_UNORM: result = 4; break;
  case VK_FORMAT_B8G8R8A8_SNORM: result = 4; break;
  case VK_FORMAT_B8G8R8A8_USCALED: result = 4; break;
  case VK_FORMAT_B8G8R8A8_SSCALED: result = 4; break;
  case VK_FORMAT_B8G8R8A8_UINT: result = 4; break;
  case VK_FORMAT_B8G8R8A8_SINT: result = 4; break;
  case VK_FORMAT_B8G8R8A8_SRGB: result = 4; break;
  case VK_FORMAT_A8B8G8R8_UNORM_PACK32: result = 4; break;
  case VK_FORMAT_A8B8G8R8_SNORM_PACK32: result = 4; break;
  case VK_FORMAT_A8B8G8R8_USCALED_PACK32: result = 4; break;
  case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: result = 4; break;
  case VK_FORMAT_A8B8G8R8_UINT_PACK32: result = 4; break;
  case VK_FORMAT_A8B8G8R8_SINT_PACK32: result = 4; break;
  case VK_FORMAT_A8B8G8R8_SRGB_PACK32: result = 4; break;
  case VK_FORMAT_A2R10G10B10_UNORM_PACK32: result = 4; break;
  case VK_FORMAT_A2R10G10B10_SNORM_PACK32: result = 4; break;
  case VK_FORMAT_A2R10G10B10_USCALED_PACK32: result = 4; break;
  case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: result = 4; break;
  case VK_FORMAT_A2R10G10B10_UINT_PACK32: result = 4; break;
  case VK_FORMAT_A2R10G10B10_SINT_PACK32: result = 4; break;
  case VK_FORMAT_A2B10G10R10_UNORM_PACK32: result = 4; break;
  case VK_FORMAT_A2B10G10R10_SNORM_PACK32: result = 4; break;
  case VK_FORMAT_A2B10G10R10_USCALED_PACK32: result = 4; break;
  case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: result = 4; break;
  case VK_FORMAT_A2B10G10R10_UINT_PACK32: result = 4; break;
  case VK_FORMAT_A2B10G10R10_SINT_PACK32: result = 4; break;
  case VK_FORMAT_R16_UNORM: result = 2; break;
  case VK_FORMAT_R16_SNORM: result = 2; break;
  case VK_FORMAT_R16_USCALED: result = 2; break;
  case VK_FORMAT_R16_SSCALED: result = 2; break;
  case VK_FORMAT_R16_UINT: result = 2; break;
  case VK_FORMAT_R16_SINT: result = 2; break;
  case VK_FORMAT_R16_SFLOAT: result = 2; break;
  case VK_FORMAT_R16G16_UNORM: result = 4; break;
  case VK_FORMAT_R16G16_SNORM: result = 4; break;
  case VK_FORMAT_R16G16_USCALED: result = 4; break;
  case VK_FORMAT_R16G16_SSCALED: result = 4; break;
  case VK_FORMAT_R16G16_UINT: result = 4; break;
  case VK_FORMAT_R16G16_SINT: result = 4; break;
  case VK_FORMAT_R16G16_SFLOAT: result = 4; break;
  case VK_FORMAT_R16G16B16_UNORM: result = 6; break;
  case VK_FORMAT_R16G16B16_SNORM: result = 6; break;
  case VK_FORMAT_R16G16B16_USCALED: result = 6; break;
  case VK_FORMAT_R16G16B16_SSCALED: result = 6; break;
  case VK_FORMAT_R16G16B16_UINT: result = 6; break;
  case VK_FORMAT_R16G16B16_SINT: result = 6; break;
  case VK_FORMAT_R16G16B16_SFLOAT: result = 6; break;
  case VK_FORMAT_R16G16B16A16_UNORM: result = 8; break;
  case VK_FORMAT_R16G16B16A16_SNORM: result = 8; break;
  case VK_FORMAT_R16G16B16A16_USCALED: result = 8; break;
  case VK_FORMAT_R16G16B16A16_SSCALED: result = 8; break;
  case VK_FORMAT_R16G16B16A16_UINT: result = 8; break;
  case VK_FORMAT_R16G16B16A16_SINT: result = 8; break;
  case VK_FORMAT_R16G16B16A16_SFLOAT: result = 8; break;
  case VK_FORMAT_R32_UINT: result = 4; break;
  case VK_FORMAT_R32_SINT: result = 4; break;
  case VK_FORMAT_R32_SFLOAT: result = 4; break;
  case VK_FORMAT_R32G32_UINT: result = 8; break;
  case VK_FORMAT_R32G32_SINT: result = 8; break;
  case VK_FORMAT_R32G32_SFLOAT: result = 8; break;
  case VK_FORMAT_R32G32B32_UINT: result = 12; break;
  case VK_FORMAT_R32G32B32_SINT: result = 12; break;
  case VK_FORMAT_R32G32B32_SFLOAT: result = 12; break;
  case VK_FORMAT_R32G32B32A32_UINT: result = 16; break;
  case VK_FORMAT_R32G32B32A32_SINT: result = 16; break;
  case VK_FORMAT_R32G32B32A32_SFLOAT: result = 16; break;
  case VK_FORMAT_R64_UINT: result = 8; break;
  case VK_FORMAT_R64_SINT: result = 8; break;
  case VK_FORMAT_R64_SFLOAT: result = 8; break;
  case VK_FORMAT_R64G64_UINT: result = 16; break;
  case VK_FORMAT_R64G64_SINT: result = 16; break;
  case VK_FORMAT_R64G64_SFLOAT: result = 16; break;
  case VK_FORMAT_R64G64B64_UINT: result = 24; break;
  case VK_FORMAT_R64G64B64_SINT: result = 24; break;
  case VK_FORMAT_R64G64B64_SFLOAT: result = 24; break;
  case VK_FORMAT_R64G64B64A64_UINT: result = 32; break;
  case VK_FORMAT_R64G64B64A64_SINT: result = 32; break;
  case VK_FORMAT_R64G64B64A64_SFLOAT: result = 32; break;
  case VK_FORMAT_B10G11R11_UFLOAT_PACK32: result = 4; break;
  case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: result = 4; break;

  default:
    break;
  }
  return result;
}



struct VertexInputDescription {

	std::vector<VkVertexInputBindingDescription> bindings;
	std::vector<VkVertexInputAttributeDescription> attributes;
};

struct DescriptorSetLayoutData {
  uint32_t setNumber;
  VkDescriptorSetLayoutCreateInfo createInfo = {};
  std::vector<VkDescriptorSetLayoutBinding> bindings;
};

struct ShaderProgram {
    //we only have vertex and fragment stages
    std::array<VkShaderModule,2> module;
    std::array<SpvReflectShaderModule,2> spvModule;

    std::vector<VkDescriptorSetLayout> GenerateDescriptorLayouts( Pipeline& pipeline );

    void Destroy( void ) {
        for(auto& current : module) {
            vkDestroyShaderModule(VulkanDevice::GetGlobalDevice(), current, nullptr);
        }
        for(auto& current: spvModule) {
            spvReflectDestroyShaderModule(&current);
        }
    }
};



bool LoadShaderModule(std::vector<const char*> shaderFiles, ShaderProgram& program) {

    //open the file. With cursor at the end
    for(int i = 0; i < shaderFiles.size(); i++) {


        std::ifstream file(shaderFiles[i], std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            ENGINE_ERROR("failed to load a shader");
            return false;
        }


        //find what the size of the file is by looking up the location of the cursor
        //because the cursor is at the end, it gives the size directly in bytes
        size_t fileSize = (size_t)file.tellg();

        //spirv expects the buffer to be on uint32, so make sure to reserve an int vector big enough for the entire file
        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        //put file cursor at beginning
        file.seekg(0);

        //load the entire file into the buffer
        file.read((char*)buffer.data(), fileSize);

        //now that the file is loaded into the buffer, we can close it
        file.close();


        //create a new shader module, using the buffer we loaded
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;

        //codeSize has to be in bytes, so multiply the ints in the buffer by size of int to know the real size of the buffer
        createInfo.codeSize = buffer.size() * sizeof(uint32_t);
        createInfo.pCode = buffer.data();

        //check that the creation goes well.
        if (vkCreateShaderModule(VulkanDevice::GetGlobalDevice(), &createInfo, nullptr, &program.module[i]) != VK_SUCCESS) {
            ENGINE_ERROR("failed to create shader module");
        }

        if(spvReflectCreateShaderModule(fileSize, buffer.data(), &program.spvModule[i]) != SPV_REFLECT_RESULT_SUCCESS){
            ENGINE_ERROR("failed to load spirv reflect modules");
        }
    }
    return true;
}


bool GetVertexDescription(std::vector<SpvReflectInterfaceVariable*> inputVars, VertexInputDescription& outDescription, Pipeline* pipeline = nullptr)
{
    //we will have just 1 vertex buffer binding, with a per-vertex rate
	VkVertexInputBindingDescription description = {};
	description.binding = 0;
	description.stride = 0;
	description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	for(int i=0; i < inputVars.size(); i++) {
        const SpvReflectInterfaceVariable& refl_var = *(inputVars[i]);
        if (refl_var.decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) {
            continue;
        }
        if (pipeline != nullptr) {
            pipeline->vertexInputs.push_back(refl_var.name);
        }
        VkVertexInputAttributeDescription attribute = {};
        attribute.binding = description.binding;
        attribute.location = refl_var.location;
        attribute.format = static_cast<VkFormat>(inputVars[i]->format);
        attribute.offset = 0;
        attributeDescriptions.push_back(attribute);
    }

    if(attributeDescriptions.size()==0) {
        return false;
    }

    std::sort(attributeDescriptions.begin(), attributeDescriptions.end(),[](const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b) {
        return a.location < b.location;
    });

    for (auto& attribute : attributeDescriptions) {
      uint32_t formatSize = FormatSize(attribute.format);
      attribute.offset = description.stride;
      description.stride += formatSize;
    }

    outDescription.attributes = attributeDescriptions;
    outDescription.bindings.push_back(description);

    return true;
}


VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule) {

    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;

    //shader stage
    info.stage = stage;
    //module containing the code for this shader stage
    info.module = shaderModule;
    //the entry point of the shader
    info.pName = "main";
    return info;
}

VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo() {
    VkPipelineLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;

    //empty defaults
    info.flags = 0;
    info.setLayoutCount = 0;
    info.pSetLayouts = nullptr;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = nullptr;
    return info;
}

VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo() {
    VkPipelineVertexInputStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.pNext = nullptr;

    //no vertex bindings or attributes
    info.vertexBindingDescriptionCount = 0;
    info.vertexAttributeDescriptionCount = 0;
    return info;
}

VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo(VkPrimitiveTopology topology) {
    VkPipelineInputAssemblyStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.pNext = nullptr;

    info.topology = topology;
    //we are not going to use primitive restart on the entire tutorial so leave it on false
    info.primitiveRestartEnable = VK_FALSE;
    return info;
}

VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo(VkPolygonMode polygonMode, bool depthEnabled)
{
    VkPipelineRasterizationStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.pNext = nullptr;

    info.depthClampEnable = static_cast<VkBool32>(depthEnabled);
    //discards all primitives before the rasterization stage if enabled which we don't want
    info.rasterizerDiscardEnable = VK_FALSE;

    info.polygonMode = polygonMode;
    info.lineWidth = 1.0f;
    //no backface cull
    info.cullMode = VK_CULL_MODE_NONE;
    info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    //no depth bias
    info.depthBiasEnable = static_cast<VkBool32>(depthEnabled);
    info.depthBiasConstantFactor = 0.0f;
    info.depthBiasClamp = 0.0f;
    info.depthBiasSlopeFactor = 0.0f;

    return info;
}

VkPipelineDepthStencilStateCreateInfo DepthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp)
{
    VkPipelineDepthStencilStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    info.pNext = nullptr;

    info.depthTestEnable = bDepthTest ? VK_TRUE : VK_FALSE;
    info.depthWriteEnable = bDepthWrite ? VK_TRUE : VK_FALSE;
    info.depthCompareOp = bDepthTest ? compareOp : VK_COMPARE_OP_ALWAYS;
    info.depthBoundsTestEnable = VK_FALSE;
    info.minDepthBounds = 0.0f; // Optional
    info.maxDepthBounds = 1.0f; // Optional
    info.stencilTestEnable = VK_FALSE;

    return info;
}



VkPipelineMultisampleStateCreateInfo MultisamplingStateCreateInfo()
{
    VkPipelineMultisampleStateCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.pNext = nullptr;

    info.sampleShadingEnable = VK_FALSE;
    //multisampling defaulted to no multisampling (1 sample per pixel)
    info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    info.minSampleShading = 1.0f;
    info.pSampleMask = nullptr;
    info.alphaToCoverageEnable = VK_FALSE;
    info.alphaToOneEnable = VK_FALSE;
    return info;
}

VkPipelineColorBlendAttachmentState ColorBlendAttachmentState() {
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    return colorBlendAttachment;
}





bool PipelineBuilder::BuildGraphicsPipeline(std::vector<const char *> files, std::vector<VkShaderStageFlagBits> shaderStageFlags, Pipeline& Pipeline) {
    if (Pipeline.pipeline != VK_NULL_HANDLE) {
        return true;
    }
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineDepthStencilStateCreateInfo depthStencil;

    ShaderProgram program;
    // layout and shader modules creation
    LoadShaderModule(files, program);

    //build the pipeline layout that controls the inputs/outputs of the shader
	//we are not using descriptor sets or other systems yet, so no need to use anything other than empty default
	VkPipelineLayoutCreateInfo PipelineLayoutInfo = PipelineLayoutCreateInfo();
    std::vector<VkDescriptorSetLayout> descriptorLayouts = program.GenerateDescriptorLayouts(Pipeline);
    if (descriptorLayouts.size() > 0) {
        PipelineLayoutInfo.pSetLayouts = descriptorLayouts.data();
        PipelineLayoutInfo.setLayoutCount = descriptorLayouts.size();
    }

    for(int i = 0; i < descriptorLayouts.size(); i++) {
        DescriptorAllocator::Allocate(Pipeline.descriptorSets[i].set, descriptorLayouts[i]);
    }

	VK_CHECK_RESULT(vkCreatePipelineLayout(VulkanDevice::GetGlobalDevice(), &PipelineLayoutInfo, nullptr, &Pipeline.pipelineLayout));

	//build the stage-create-info for both vertex and fragment stages. This lets the pipeline know the shader modules per stage
    for(int i = 0; i < shaderStageFlags.size(); i++) {
        shaderStages.push_back(PipelineShaderStageCreateInfo(shaderStageFlags[i],program.module[i]));
    }
	//vertex input controls how to read vertices from vertex buffers. We aren't using it yet
	vertexInputInfo = VertexInputStateCreateInfo();

    VertexInputDescription vertexDescription = {};
    for(auto& currentModule: program.spvModule) {
        if (currentModule.shader_stage == static_cast<VkShaderStageFlags>(VK_SHADER_STAGE_VERTEX_BIT)) {
            // Enumerate and extract shader's input variables
            uint32_t varCount = 0;
            SpvReflectResult result = spvReflectEnumerateInputVariables(&currentModule, &varCount, nullptr);
            assert(result == SPV_REFLECT_RESULT_SUCCESS);
            std::vector<SpvReflectInterfaceVariable*> inputVars(varCount);
            result = spvReflectEnumerateInputVariables(&currentModule, &varCount, inputVars.data());
            assert(result == SPV_REFLECT_RESULT_SUCCESS);
            vertexDescription = {};
            if (GetVertexDescription(inputVars, vertexDescription, &Pipeline)) {
                vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
                vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
                vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
                vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
                break;
            }
        }
        break;
    }

	//input assembly is the configuration for drawing triangle lists, strips, or individual points.
	//we are just going to draw triangle list
	inputAssembly = InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	//configure the rasterizer to draw filled triangles
	rasterizer = RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, false);

	//we don't use multisampling, so just run the default one
	multisampling = MultisamplingStateCreateInfo();

	//a single blend attachment with no blending and writing to RGBA
	colorBlendAttachment = ColorBlendAttachmentState();

    depthStencil = DepthStencilCreateInfo(true,true, VK_COMPARE_OP_LESS_OR_EQUAL);

    //make viewport state from our stored viewport and scissor.
    //at the moment we won't support multiple viewports or scissors
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &Pipeline.viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &Pipeline.scissor;

    //setup dummy color blending. We aren't using transparent objects yet
    //the blending is just "no blend", but we do write to the color attachment
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;


    //dynamic states
    std::vector <VkDynamicState> dStates = {VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR};
    if(rasterizer.depthBiasEnable == true)
    {
        dStates.push_back(VK_DYNAMIC_STATE_DEPTH_BIAS);
    }
    VkPipelineDynamicStateCreateInfo dStateInfo = {};
    dStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dStateInfo.dynamicStateCount = dStates.size();
    dStateInfo.pDynamicStates = dStates.data();

    // New create info to define color, depth and stencil attachments at pipeline create time
    VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo = {};
    pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipelineRenderingCreateInfo.colorAttachmentCount = 1;
    pipelineRenderingCreateInfo.pColorAttachmentFormats = &VulkanDevice::GetCurrentSwapchainImageFormat();

    if(Pipeline.depthEnabled) {
        pipelineRenderingCreateInfo.depthAttachmentFormat = VulkanDevice::GetDepthFormat();
        pipelineRenderingCreateInfo.stencilAttachmentFormat =  VulkanDevice::GetDepthFormat();
    }
    pipelineRenderingCreateInfo.viewMask = 0;

    //build the actual pipeline
	//we now use all of the info structs we have been writing into into this one to create the pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.layout = Pipeline.pipelineLayout;
    pipelineInfo.pDynamicState = &dStateInfo;
	pipelineInfo.renderPass = VK_NULL_HANDLE;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.pNext = &pipelineRenderingCreateInfo;

	//it's easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
	if (vkCreateGraphicsPipelines(VulkanDevice::GetGlobalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &Pipeline.pipeline) == VK_SUCCESS) {
        program.Destroy();
		return true;
	}
    ENGINE_ERROR("failed to create a graphics pipeline");
    return false;
}


void Pipeline::DestroyPipeline() {
    vkDestroyPipelineLayout(VulkanDevice::GetGlobalDevice(), pipelineLayout, nullptr);
    vkDestroyPipeline(VulkanDevice::GetGlobalDevice(), pipeline, nullptr);
}





VkDescriptorPool CreatePool(const DescriptorAllocator::PoolSizes& pool_sizes, int count, VkDescriptorPoolCreateFlags flags) {
    std::vector<VkDescriptorPoolSize> sizes;
    sizes.reserve(pool_sizes.sizes.size());

    for (auto& sz : pool_sizes.sizes){
        sizes.push_back({sz.first, uint32_t(sz.second * count)});
    }

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = flags;
    pool_info.maxSets = count;
    pool_info.poolSizeCount = (uint32_t)sizes.size();
    pool_info.pPoolSizes = sizes.data();

    VkDescriptorPool descriptorPool;
    vkCreateDescriptorPool(VulkanDevice::GetGlobalDevice(), &pool_info, nullptr, &descriptorPool);

    return descriptorPool;
}

VkDescriptorPool DescriptorAllocator::GrabPool() {

    //if there are free pools available
    if(freePools.size() > 0) {
        //grab pool from the back of the vector and remove it from there.
		VkDescriptorPool pool = freePools.back();
		freePools.pop_back();
		return pool;
    }
    else
	{
		//no pools availible, so create a new one
		return CreatePool(descriptorSizes, 1000, VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT);
	}
}

bool DescriptorAllocator::Allocate(VkDescriptorSet& set, VkDescriptorSetLayout layout)
{
    //initialize the currentPool handle if it's null
    if (currentPool == VK_NULL_HANDLE){

        currentPool = GrabPool();
        usedPools.push_back(currentPool);
    }

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;

    allocInfo.pSetLayouts = &layout;
    allocInfo.descriptorPool = currentPool;
    allocInfo.descriptorSetCount = 1;

    //try to allocate the descriptor set
    VkResult allocResult = vkAllocateDescriptorSets(VulkanDevice::GetGlobalDevice(), &allocInfo, &set);
    bool needReallocate = false;

    switch (allocResult) {
    case VK_SUCCESS:
        //all good, return
        return true;
    case VK_ERROR_FRAGMENTED_POOL:
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        //reallocate pool
        needReallocate = true;
        break;
    default:
        //unrecoverable error
        return false;
    }

    if (needReallocate){
        //allocate a new pool and retry
        currentPool = GrabPool();
        usedPools.push_back(currentPool);

        allocResult = vkAllocateDescriptorSets(VulkanDevice::GetGlobalDevice(), &allocInfo, &set);

        //if it still fails then we have big issues
        if (allocResult == VK_SUCCESS){
            ENGINE_FATAL("failed to allocate descriptor sets");
            return true;
        }
    }

    return false;
}

void DescriptorAllocator::ResetPools(){
	//reset all used pools and add them to the free pools
	for (auto p : usedPools){
		vkResetDescriptorPool(VulkanDevice::GetGlobalDevice(), p, 0);
		freePools.push_back(p);
	}

	//clear the used pools, since we've put them all in the free pools
	usedPools.clear();

	//reset the current pool handle back to null
	currentPool = VK_NULL_HANDLE;
}

void DescriptorAllocator::CleanUp() {
    for (int i = 0; i < freePools.size(); i++){
        vkDestroyDescriptorPool(VulkanDevice::GetGlobalDevice(), freePools[i], nullptr);
    }
    for (int i = 0; i < usedPools.size(); i++){
        vkDestroyDescriptorPool(VulkanDevice::GetGlobalDevice(), usedPools[i], nullptr);
    }
}




VkDescriptorSetLayout DescriptorLayoutCache::CreateDescriptorLayout( VkDescriptorSetLayoutCreateInfo& info ){
	DescriptorLayoutInfo layoutInfo;
	layoutInfo.bindings.reserve(info.bindingCount);
	bool isSorted = true;
	int lastBinding = -1;

	//copy from the direct info struct into our own one
	for (int i = 0; i < info.bindingCount; i++) {
		layoutInfo.bindings.push_back(info.pBindings[i]);

		//check that the bindings are in strict increasing order
		if (info.pBindings[i].binding > lastBinding){
			lastBinding = info.pBindings[i].binding;
		}
		else{
			isSorted = false;
		}
	}
	//sort the bindings if they aren't in order
	if (!isSorted){
		std::sort(layoutInfo.bindings.begin(), layoutInfo.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b ){
				return a.binding < b.binding;
		});
	}

	//try to grab from cache
	auto it = layoutCache.find(layoutInfo);
	if (it != layoutCache.end()){
		return (*it).second;
	}
	else {
        //create a new one (not found)
        VkDescriptorSetLayout layout;
        vkCreateDescriptorSetLayout(VulkanDevice::GetGlobalDevice(), &info, nullptr, &layout);

        //add to cache
        layoutCache[layoutInfo] = layout;
        return layout;
    }
}

bool DescriptorLayoutCache::DescriptorLayoutInfo::operator==( const DescriptorLayoutInfo& other ) const{
    if (other.flags != flags) {
        return false;
    }
	if (other.bindings.size() != bindings.size()){
		return false;
	}
	else {
		//compare each of the bindings is the same. Bindings are sorted so they will match
		for (int i = 0; i < bindings.size(); i++) {
			if (other.bindings[i].binding != bindings[i].binding){
				return false;
			}
			if (other.bindings[i].descriptorType != bindings[i].descriptorType){
				return false;
			}
			if (other.bindings[i].descriptorCount != bindings[i].descriptorCount){
				return false;
			}
			if (other.bindings[i].stageFlags != bindings[i].stageFlags){
				return false;
			}
		}
		return true;
	}
}

size_t DescriptorLayoutCache::DescriptorLayoutInfo::Hash() const{
    using std::size_t;
    using std::hash;

    size_t result = hash<size_t>()(bindings.size());

    for (const VkDescriptorSetLayoutBinding& b : bindings)
    {
        //pack the binding data into a single int64. Not fully correct but it's ok
        size_t binding_hash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;

        //shuffle the packed binding data and xor it with the main hash
        result ^= hash<size_t>()(binding_hash);
    }

    return result;
}

void DescriptorLayoutCache::CleanUp( void )
{
    //delete every descriptor layout held
    for (auto pair : layoutCache)
    {
        vkDestroyDescriptorSetLayout(VulkanDevice::GetGlobalDevice(), pair.second, nullptr);
    }
}

std::vector<VkDescriptorSetLayout> ShaderProgram::GenerateDescriptorLayouts( Pipeline& pipeline ) {
    std::vector<VkPipelineShaderStageCreateInfo> shader_stages = {};
    std::vector<int> setNumbers;
    std::vector<DescriptorSetLayoutData> descriptorLayouts = {};
    for(auto& currentModule : spvModule) {
        uint32_t count = 0;
        SpvReflectResult result = spvReflectEnumerateDescriptorSets(&currentModule, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectDescriptorSet*> sets(count);
        result = spvReflectEnumerateDescriptorSets(&currentModule, &count, sets.data());
        assert(result == SPV_REFLECT_RESULT_SUCCESS);

        std::vector<DescriptorSetLayoutData> setLayouts(sets.size(), DescriptorSetLayoutData{});
        for (size_t i_set = 0; i_set < sets.size(); ++i_set) {
            const SpvReflectDescriptorSet& refl_set = *(sets[i_set]);
            DescriptorSetLayoutData& layout = setLayouts[i_set];
            layout.bindings.resize(refl_set.binding_count);
            for (uint32_t i_binding = 0; i_binding < refl_set.binding_count; ++i_binding) {
                const SpvReflectDescriptorBinding& refl_binding = *(refl_set.bindings[i_binding]);
                VkDescriptorSetLayoutBinding& layout_binding = layout.bindings[i_binding];
                layout_binding.binding = refl_binding.binding;
                layout_binding.descriptorType = static_cast<VkDescriptorType>(refl_binding.descriptor_type);
                layout_binding.descriptorCount = 1;
                for (uint32_t i_dim = 0; i_dim < refl_binding.array.dims_count; ++i_dim) {
                    layout_binding.descriptorCount *= refl_binding.array.dims[i_dim];
                }
                layout_binding.stageFlags = static_cast<VkShaderStageFlagBits>(currentModule.shader_stage);
            }


            setNumbers.push_back(refl_set.set);
            layout.setNumber = refl_set.set;
            layout.createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layout.createInfo.bindingCount = refl_set.binding_count;
            layout.createInfo.pBindings = layout.bindings.data();

            descriptorLayouts.push_back(layout);
        }
    }

    std::sort(setNumbers.begin(), setNumbers.end());
    std::vector<int>::iterator iterator;
    iterator = std::unique(setNumbers.begin(), setNumbers.end());

    setNumbers.resize(std::distance(setNumbers.begin(), iterator));

    std::vector<DescriptorSetLayoutData> mergedLayouts;
    mergedLayouts.resize(descriptorLayouts.size());
	std::vector<VkDescriptorSetLayout> layouts;
	for (int i = 0; i < mergedLayouts.size(); i++) {
		DescriptorSetLayoutData& ly = mergedLayouts[i];

		ly.setNumber = i;

		ly.createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		std::unordered_map<int,VkDescriptorSetLayoutBinding> binds;
		for (auto& s : descriptorLayouts) {
			if (s.setNumber == i) {
				for (auto& b : s.bindings)
				{
					auto it = binds.find(b.binding);
					if (it == binds.end())
					{
						binds[b.binding] = b;
						//ly.bindings.push_back(b);
					}
					else {
						//merge flags
						binds[b.binding].stageFlags |= b.stageFlags;
					}

				}
			}
		}
		for (auto [k, v] : binds)
		{
			ly.bindings.push_back(v);
		}
		//sort the bindings, for hash purposes
		std::sort(ly.bindings.begin(), ly.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
			return a.binding < b.binding;
		});

        std::vector<VkDescriptorBindingFlags> flags;
        for(auto& currentBinding : ly.bindings){
            if(currentBinding.descriptorType != VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE) {
                flags.push_back(0);
            }
            else {
                flags.push_back(VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT);
            }
        }
        VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlags{};
        bindingFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
        bindingFlags.bindingCount = (uint32_t)ly.bindings.size();
        bindingFlags.pBindingFlags = flags.data();

		ly.createInfo.bindingCount = (uint32_t)ly.bindings.size();
		ly.createInfo.pBindings = ly.bindings.data();
		ly.createInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
		ly.createInfo.pNext = &bindingFlags;


		layouts.push_back(DescriptorLayoutCache::CreateDescriptorLayout(ly.createInfo));
	}

    for(auto& layout : mergedLayouts) {
        Pipeline::DescriptorInfo descriptorInfo;
        descriptorInfo.bindingInfo.resize(layout.bindings.size());
        for(int i = 0; i < layout.bindings.size(); i++) {
            descriptorInfo.bindingInfo[i].shaderStageFlags = layout.bindings[i].stageFlags;
            descriptorInfo.bindingInfo[i].descriptorTypes = layout.bindings[i].descriptorType;
            descriptorInfo.bindingInfo[i].binding = layout.bindings[i].binding;
        }

        pipeline.descriptorSets.push_back(descriptorInfo);
    }

    return layouts;
}
