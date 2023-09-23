#include "cepch.h"
#include "VulkanDevice.h"

#include "VulkanInstance.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    void VulkanDevice::Init(DeviceRequirements requirements, VulkanInstance* instance)
    {
        m_Instance = instance;

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
            }
            
            if (suitable && !m_PhysicalDevice) {
                m_PhysicalDevice = device;
                m_QueueIndices = queueIndices;
                break;
            }
        }

        CE_ASSERT(m_PhysicalDevice);
        CE_INFO("Picked %s", m_PhysicalDevice.getProperties().deviceName);
        CE_TRACE("Selected Queue indices:");
        CE_TRACE("\t Graphics: %d", m_QueueIndices.Graphics);
        CE_TRACE("\t Transfer: %d", m_QueueIndices.Transfer);
        CE_TRACE("\t Compute: %d", m_QueueIndices.Compute);
        CE_TRACE("\t Sparse: %d", m_QueueIndices.Sparse);
    }

    void VulkanDevice::Shutdown()
    {
    }
}
