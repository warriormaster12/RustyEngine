#pragma once

#include <cstdint>
#define VK_NO_PROTOTYPES
#include "third-party/volk/volk.h"

#include "third-party/vma/vk_mem_alloc.h"
#include <assert.h>

#include <vector>

#include "logger.h"

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		ENGINE_FATAL("Fatal : VkResult is {0} in {1} at line {2}", res, __FILE__, __LINE__); \
		assert(res == VK_SUCCESS);																		\
	}																									\
}


namespace VkTools {

	struct AllocatedImage {
		VkImage image = VK_NULL_HANDLE;
		VmaAllocation allocation;
		VkImageView defaultView = VK_NULL_HANDLE;
        VkFormat imageFormat;
		int mipLevels = 0;

		void DestroyImage( void );
	};

	class AllocatedBuffer {
	private:
		uint32_t bufferSize = 0;
		VmaAllocation allocation = VK_NULL_HANDLE;
	public:
		VkBuffer buffer = VK_NULL_HANDLE;
		VkDescriptorBufferInfo descBuffInfo = {};

		void AllocateBuffer( const VkBufferUsageFlags& usage, VmaMemoryUsage memoryUsage, const uint32_t& dataSize, const uint32_t& typeSize );

		void UploadBufferData( const void* pdata, const uint32_t& offset = 0 );

		void DestroyBuffer( void );
	};

	void InsertImageMemoryBarrier( VkCommandBuffer& cmdbuffer, VkImage& image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange );

	VkImageMemoryBarrier ImageMemoryBarrier( void );

	VkCommandPoolCreateInfo CommandPoolCreateInfo( uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0 );

	VkCommandBufferAllocateInfo CommandBufferAllocateInfo( VkCommandPool& pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY );

	VkCommandBufferBeginInfo CommandBufferBeginInfo( VkCommandBufferUsageFlags flags = 0 );

	VkFenceCreateInfo FenceCreateInfo( VkFenceCreateFlags flags = 0 );

	VkSemaphoreCreateInfo SemaphoreCreateInfo( void );

	VkSubmitInfo SubmitInfo( VkCommandBuffer* p_cmd );

	VkPresentInfoKHR PresentInfo( void );

}
