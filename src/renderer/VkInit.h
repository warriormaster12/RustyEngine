#pragma once

#include "VkTools.h"
#define VK_NO_PROTOTYPES
#include "third-party/vk-bootstrap/VkBootstrap.h"
#include "third-party/volk/volk.h"

#include "third-party/vma/vk_mem_alloc.h"


struct PerFrameData {
    VkCommandBuffer commandBuffer;
    VkFence renderFence;
    VkSemaphore presentSemaphore, renderSemaphore;
    VkCommandPool commandPool;

    void CleanUp(VkDevice& device) {
        if(commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device,commandPool, nullptr);
        }

        if(renderFence != VK_NULL_HANDLE) {
            vkDestroyFence(device, renderFence, nullptr);
        }

        if(presentSemaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, presentSemaphore, nullptr);
        }

        if(renderSemaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, renderSemaphore, nullptr);
        }

    }
};



class VulkanDevice {
public:
    static void        Init();
    static void        CreateDevice();
    static void        CreateSwapchain( uint32_t width, uint32_t height );
    static void        Shutdown();
    static VmaAllocator& GetGlobalMemoryAllocator() {return allocator;}
    static VkInstance& GetGlobalInstance() {return instance;}
    static VkDevice& GetGlobalDevice() {return device;}
    static VkSurfaceKHR& GetGlobalSurface() {return surface;}
    static VkSwapchainKHR& GetGlobalSwapchain() {return swapchain;}
    static VkQueue& GetGraphicsQueue() {return graphicsQueue;}
    static VkExtent2D& GetSwapchainExtent() {return swapchainExtent;}
    static VkImageView& GetCurrentSwapchainImageView( uint32_t& imageIndex ) {return swapchainImageViews[imageIndex];}
    static VkImage&     GetCurrentSwapchainImage( uint32_t& imageIndex ) {return swapchainImages[imageIndex];}
    static VkFormat&    GetCurrentSwapchainImageFormat() {return swapchainImageFormat;}
    static VkImage&     GetDepthImage() {return depthImage.image;}
    static VkImageView& GetDepthImageView() {return depthImage.defaultView;}
    static VkFormat&    GetDepthFormat() {return depthImage.imageFormat;}

    static PerFrameData& GetCurrentFrame( int frameNumber ) { return perFrameData[frameNumber % frameOverlap];}
    static int GetFrameOverlap() {return frameOverlap;}


    static inline bool vkInitialized = false;
private:
    static void CreateCommandBuffer();

    static void CreateSyncObjects();

    static inline VkInstance instance;
    static inline VkDebugUtilsMessengerEXT debugMessenger;
    static inline VkSurfaceKHR surface;
    static inline VkDevice device;
    static inline VkPhysicalDevice physicalDevice;
    static inline VkPhysicalDeviceProperties gpuProperties;

    static inline VkQueue graphicsQueue;
    static inline uint32_t graphicsQueueFamily;

    static inline VmaAllocator allocator;

    //swapchain
    static inline VkSwapchainKHR swapchain;
    static inline VkExtent2D swapchainExtent;
    static inline std::vector<VkImage> swapchainImages;
    static inline std::vector<VkImageView> swapchainImageViews;
    static inline VkFormat swapchainImageFormat;

    static inline VkTools::AllocatedImage depthImage;

    static inline const int frameOverlap = 2;
    static inline PerFrameData perFrameData[frameOverlap];
};

/*
===============
PipelineBuilder
===============
*/
struct Pipeline {
	VkPipeline pipeline = VK_NULL_HANDLE;
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineLayout pipelineLayout;
    std::vector<std::string> vertexInputs;
    bool depthEnabled = true;
    bool bound = false;
    struct DescriptorInfo {
        struct BindingInfo {
            VkShaderStageFlags shaderStageFlags;
            VkDescriptorType descriptorTypes;
            uint32_t binding;
        };

        std::vector<BindingInfo> bindingInfo;
        VkDescriptorSet set = VK_NULL_HANDLE;

        struct DescriptorBufferImageInfo {
            VkDescriptorBufferInfo buffInfo = {};
            VkDescriptorImageInfo imageInfo = {};
        };

        std::vector<DescriptorBufferImageInfo> descriptorBufferImageInfos;
        void GenerateDescriptorBuffers( VkDescriptorBufferInfo& buffInfo ) {
            DescriptorBufferImageInfo info = {};
            info.buffInfo = buffInfo;
            descriptorBufferImageInfos.push_back(info);
            std::vector<VkWriteDescriptorSet> writes;
            for(auto& binding : bindingInfo) {
                VkWriteDescriptorSet  writeDescriptorSets{};
                writeDescriptorSets.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSets.dstSet = set;
                writeDescriptorSets.dstBinding = binding.binding;
                writeDescriptorSets.descriptorType = binding.descriptorTypes;
                writeDescriptorSets.descriptorCount = 1;
                if (descriptorBufferImageInfos.size() > 0) {
                    if ( binding.descriptorTypes == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || binding.descriptorTypes == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER) {
                        writeDescriptorSets.pBufferInfo = &descriptorBufferImageInfos[binding.binding].buffInfo;
                    }
                }
                writes.push_back(writeDescriptorSets);
            }
            vkUpdateDescriptorSets(VulkanDevice::GetGlobalDevice(), writes.size(), writes.data(), 0, nullptr);
        }
        void GenerateDescriptorImages( VkDescriptorImageInfo& imageInfo ) {
            DescriptorBufferImageInfo info = {};
            info.imageInfo = imageInfo;
            descriptorBufferImageInfos.push_back(info);
        }
    };
    std::vector<DescriptorInfo> descriptorSets;


    void DestroyPipeline();
};

class PipelineBuilder {
public:
	static bool BuildGraphicsPipeline(std::vector<const char*> files, std::vector<VkShaderStageFlagBits> shaderStageFlags, Pipeline& idpipeline);
};

class VulkanRBE {
public:
    static void        PrepareFrame();
    static void        SubmitFrame();

    //framebuffer and renderpass replacement
    static void        BeginRenderLayer( uint32_t width=0, uint32_t height=0  );
    static void        BindVertexBuffer( const uint32_t& firstBinding, const uint32_t& bindingCount, VkTools::AllocatedBuffer& buffer, const VkDeviceSize& offset =0 );
    static void        BindIndexBuffer( VkTools::AllocatedBuffer& buffer, const VkDeviceSize& offset =0  );
    static void        Draw( const uint32_t& vertexCount, const uint32_t& instanceCount = 1, const uint32_t& firstVertex = 0 , const uint32_t& firstInstance = 0 );
    static void        DrawIndexed( const uint32_t& indexCount, const uint32_t& instanceCount = 1, const uint32_t& firstIndex = 0 , const int32_t& vertexOffset = 0 , const uint32_t& firstInstance = 0  );
    static void        EndRenderLayer();

    static void        CleanUp();

    static inline int frameCount;

    static inline float clearColor[4];

    static inline Pipeline* currentPipeline = nullptr;
};


//Descriptor set allocation and layout caching

class DescriptorAllocator {
public:
    struct PoolSizes {
        static inline std::vector<std::pair<VkDescriptorType,float>> sizes =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
        };
    };

    static void ResetPools();
    static bool Allocate(VkDescriptorSet& set, VkDescriptorSetLayout layout);

    static void CleanUp();
private:
    static VkDescriptorPool GrabPool();

    static inline VkDescriptorPool currentPool{VK_NULL_HANDLE};
    static inline PoolSizes descriptorSizes;
    static inline std::vector<VkDescriptorPool> usedPools;
    static inline std::vector<VkDescriptorPool> freePools;

};

class DescriptorLayoutCache {
public:
    static VkDescriptorSetLayout CreateDescriptorLayout( VkDescriptorSetLayoutCreateInfo& info );
    static void CleanUp();
private:
    struct DescriptorLayoutInfo {
        //good idea to turn this into a inlined array
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        VkDescriptorSetLayoutCreateFlags flags;

        bool operator==( const DescriptorLayoutInfo& other ) const;

        size_t Hash() const;
    };

    struct DescriptorLayoutHash		{

        std::size_t operator()(const DescriptorLayoutInfo& k) const{
            return k.Hash();
        }
    };

    static inline std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> layoutCache;
};
