#pragma once

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <vulkan/vulkan.h>
#include "../Includes/VulkanTools.hpp"
#include "../Includes/VulkanDevice.hpp"
#include "../Includes/VulkanInstance.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct _SwapChainBuffers {
	VkImage image;
	VkImageView view;
} SwapChainBuffer;

class VulkanDevice;

namespace vks {
	class VulkanSwapChain
	{
	private:

		vks::VulkanInstance& _instance;

		//VkDevice device;
		//VkPhysicalDevice physicalDevice;
		vks::VulkanDevice& _vulkanDevice;
		//VkExtent2D _Extent;
		VkSurfaceKHR _Surface;

	public:
		VkFormat _ColorFormat;
		VkColorSpaceKHR _ColorSpace;
		VkSwapchainKHR _SwapChain = VK_NULL_HANDLE;
		uint32_t _imageCount;
		std::vector<VkImage> _Images;
		std::vector<SwapChainBuffer> _buffers;
		uint32_t _queueNodeIndex = UINT32_MAX;

		VulkanSwapChain(vks::VulkanInstance& instance, vks::VulkanDevice& vulkanDevice, VkSurfaceKHR surface);
		//~VulkanSwapChain();

		void initSurface(GLFWwindow* platformHandle);

		/* Set the Vulkan objects required for swapchain creation and management, must be called before swapchain creation */
		//void setContext(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);

		VkResult acquireNextImage(VkFence waitFence, VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		/**
		* Create the swapchain and get its images with given width and height
		*
		* @param width Pointer to the width of the swapchain (may be adjusted to fit the requirements of the swapchain)
		* @param height Pointer to the height of the swapchain (may be adjusted to fit the requirements of the swapchain)
		* @param vsync (Optional, default = false) Can be used to force vsync-ed rendering (by using VK_PRESENT_MODE_FIFO_KHR as presentation mode)
		*/
		void create(uint32_t& width, uint32_t& height, bool vsync = false);
		
		/**
		* Queue an image for presentation
		*
		* @param queue Presentation queue for presenting the image
		* @param imageIndex Index of the swapchain image to queue for presentation
		* @param waitSemaphore (Optional) Semaphore that is waited on before the image is presented (only used if != VK_NULL_HANDLE)
		*
		* @return VkResult of the queue presentation
		*/
		VkResult queuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);
		/* Free all Vulkan resources acquired by the swapchain */

		void cleanup();
	};




}

