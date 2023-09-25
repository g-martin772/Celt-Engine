#include "cepch.h"
#include "VulkanDevice.h"

#include "VulkanInstance.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    static void AddQueueToCreateInfo(std::vector<vk::DeviceQueueCreateInfo>& queueInfos, uint32_t queueFamilyIndex, uint32_t* resultIndex)
    {
        for (uint32_t i = 0; i < queueInfos.size(); i++) {
            if (queueInfos[i].queueFamilyIndex == queueFamilyIndex) {
                *resultIndex = i;
                return;
            }
        }

        vk::DeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        constexpr float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueInfos.push_back(queueCreateInfo);
        *resultIndex = queueInfos.size() - 1;
    }
    
    void VulkanDevice::Init(const DeviceRequirements& requirements, VulkanInstance* instance)
    {
        m_Instance = instance;
        m_Requirements = requirements;

        std::vector<vk::PhysicalDevice> devices = instance->GetInstance().enumeratePhysicalDevices();

        // Picking a physical device
        CE_INFO("Available Graphics Devices:");
        for (const auto& device : devices) {
            bool suitable = true;
            vk::PhysicalDeviceProperties properties = device.getProperties();
            vk::PhysicalDeviceFeatures features = device.getFeatures();
            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.getQueueFamilyProperties();
            
            CE_INFO("\t%s", properties.deviceName);
            CE_TRACE("\t\t Driver Version: %d",
                VK_VERSION_MAJOR(properties.driverVersion),
                VK_VERSION_MINOR(properties.driverVersion),
                VK_VERSION_PATCH(properties.driverVersion));
            CE_TRACE("\t\t Type: %s", vk::to_string(properties.deviceType).c_str());
            CE_TRACE("\t\t VendorId: %d", properties.vendorID);
            CE_TRACE("\t\t Max Viewports: %d", properties.limits.maxViewports);
            CE_TRACE("\t\t Max Framebuffer Size: %d, %d", properties.limits.maxFramebufferHeight, properties.limits.maxFramebufferWidth);
            CE_TRACE("\t\t Max Descriptorsets: %d", properties.limits.maxBoundDescriptorSets);
            CE_TRACE("\t\t Max memory allocations: %d", properties.limits.maxMemoryAllocationCount);
            CE_TRACE("\t\t Max dynamic Storage Buffers: %d", properties.limits.maxDescriptorSetStorageBuffersDynamic);
            CE_TRACE("\t\t Max dynamic Uniform Buffers: %d", properties.limits.maxDescriptorSetUniformBuffersDynamic);
            CE_TRACE("\t\t Max Storage Buffers: %d", properties.limits.maxDescriptorSetStorageBuffers);
            CE_TRACE("\t\t Max Uniform Buffers: %d", properties.limits.maxDescriptorSetUniformBuffers);
            CE_TRACE("\t\t Max Sampler: %d", properties.limits.maxDescriptorSetSamplers);
            CE_TRACE("\t\t Max input attachments: %d", properties.limits.maxDescriptorSetInputAttachments);
            CE_TRACE("\t\t Max sampled images: %d", properties.limits.maxDescriptorSetSampledImages);
            CE_TRACE("\t\t Max draw index: %d", properties.limits.maxDrawIndexedIndexValue);

            CE_TRACE("\t\t Supported device features:");
            CE_TRACE("\t\t\t TessellationShader: %s", (features.tessellationShader ? "Supported" : "Not Supported"));
            CE_TRACE("\t\t\t GeometryShader: %s", (features.geometryShader ? "Supported" : "Not Supported"));
            CE_TRACE("\t\t\t MultiViewport: %s", (features.multiViewport ? "Supported" : "Not Supported"));
            CE_TRACE("\t\t\t SamplerAnisotropy: %s", (features.samplerAnisotropy ? "Supported" : "Not Supported"));
            CE_TRACE("\t\t\t FillModeNonSolid: %s", (features.fillModeNonSolid ? "Supported" : "Not Supported"));
            CE_TRACE("\t\t\t SparseBinding: %s", (features.sparseBinding ? "Supported" : "Not Supported"));

            // Finding suitable queue indices
            VulkanQueueIndices queueIndices;
            
            CE_TRACE("\t\t Supported queues:");
            for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyProperties.size(); ++queueFamilyIndex) {
                const vk::QueueFamilyProperties& queueFamily = queueFamilyProperties[queueFamilyIndex];

                CE_TRACE("\t\t\t Queue Family Index: %d", queueFamilyIndex);
                CE_TRACE("\t\t\t\t Queue Count:: %d", queueFamily.queueCount);
                CE_TRACE("\t\t\t\t Queue Flags: %s", vk::to_string(queueFamily.queueFlags).c_str());
                
                if(requirements.Graphics && queueIndices.Graphics == -1 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                    queueIndices.Graphics = queueFamilyIndex;
                }

                if(requirements.Compute && queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
                    queueIndices.Compute = queueFamilyIndex;
                }

                if(requirements.Transfer && queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) {
                    queueIndices.Transfer = queueFamilyIndex;
                }

                if(requirements.Sparse && queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding) {
                    queueIndices.Sparse = queueFamilyIndex;
                }

                if(requirements.Present && device.getSurfaceSupportKHR(queueFamilyIndex, requirements.Surface)) {
                    queueIndices.Present = queueFamilyIndex;
                }
            }
            
            if (suitable && !m_PhysicalDevice) {
                m_PhysicalDevice = device;
                m_QueueIndices = queueIndices;
                break;
            }
        }

        CE_ASSERT(m_PhysicalDevice);
        CE_INFO("Picked %s", m_PhysicalDevice.getProperties().deviceName);

        CE_ASSERT(!requirements.Graphics || requirements.Graphics && m_QueueIndices.Graphics != -1);
        CE_ASSERT(!requirements.Transfer || requirements.Transfer && m_QueueIndices.Transfer != -1);
        CE_ASSERT(!requirements.Compute || requirements.Compute && m_QueueIndices.Compute != -1);
        CE_ASSERT(!requirements.Sparse || requirements.Sparse && m_QueueIndices.Sparse != -1);
        CE_ASSERT(!requirements.Present || requirements.Present && m_QueueIndices.Present != -1);
 
        std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;
        if(requirements.Graphics)
            AddQueueToCreateInfo(deviceQueueInfos, m_QueueIndices.Graphics, &m_GraphicsIndex);
        if(requirements.Transfer)
            AddQueueToCreateInfo(deviceQueueInfos, m_QueueIndices.Transfer, &m_TransferIndex);
        if(requirements.Compute)
            AddQueueToCreateInfo(deviceQueueInfos, m_QueueIndices.Compute, &m_ComputeIndex);
        if(requirements.Sparse)
            AddQueueToCreateInfo(deviceQueueInfos, m_QueueIndices.Sparse, &m_SparseIndex);
        if(requirements.Present)
            AddQueueToCreateInfo(deviceQueueInfos, m_QueueIndices.Present, &m_PresentIndex);

        CE_TRACE("Selected Queue indices:");
        CE_TRACE("\t Graphics: %d (%d)", m_QueueIndices.Graphics, m_GraphicsIndex);
        CE_TRACE("\t Transfer: %d (%d)", m_QueueIndices.Transfer, m_TransferIndex);
        CE_TRACE("\t Compute: %d (%d)", m_QueueIndices.Compute, m_ComputeIndex);
        CE_TRACE("\t Sparse: %d (%d)", m_QueueIndices.Sparse, m_SparseIndex);
        CE_TRACE("\t Present: %d (%d)", m_QueueIndices.Present, m_PresentIndex);
        
        // Logical device
        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.queueCreateInfoCount = deviceQueueInfos.size();
        deviceCreateInfo.pQueueCreateInfos = deviceQueueInfos.data();

        vk::PhysicalDeviceFeatures deviceFeatures;
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

        std::vector<const char*> deviceExtensions = {
            "VK_KHR_swapchain"
        };
        deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        
        try {
            m_Device = m_PhysicalDevice.createDevice(deviceCreateInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create logical device: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created logical device: succesful");

        // Retrieving queue handles
        for (auto& queueInfo : deviceQueueInfos)
        {
            vk::Queue queue = m_Device.getQueue(queueInfo.queueFamilyIndex, 0);
            m_Queues.push_back(queue);
        }

        // Get Swapchain Formats
        m_SurfaceCapabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(requirements.Surface);
        m_SurfaceFormats =  m_PhysicalDevice.getSurfaceFormatsKHR(requirements.Surface);
        m_SurfacePresentModes = m_PhysicalDevice.getSurfacePresentModesKHR(requirements.Surface);
        
        // Get depth format
        std::vector<vk::Format> depthFormats = {
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD32Sfloat,
            vk::Format::eD24UnormS8Uint,
            vk::Format::eD16UnormS8Uint,
            vk::Format::eD16Unorm
        };

        auto flags = vk::FormatFeatureFlagBits::eDepthStencilAttachment;
        for (auto& depthFormat : depthFormats) {
            auto props = m_PhysicalDevice.getFormatProperties(depthFormat);
            if((props.linearTilingFeatures & flags) == flags) {
                m_DepthFormat = depthFormat;
                break;
            } else if ((props.optimalTilingFeatures & flags) == flags) {
                m_DepthFormat = depthFormat;
                break;
            }
        }
    }

    void VulkanDevice::Shutdown()
    {
        m_Device.destroy();
    }
}
