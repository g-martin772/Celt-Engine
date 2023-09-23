#include "cepch.h"
#include "VulkanInstance.h"

#include "Core/Application.h"
#include "Core/Logger.h"

#if defined(CE_WINDOWS) || defined(CE_LINUX)
#include <gLFW/glfw3.h>
#endif

namespace CeltEngine
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        switch (messageSeverity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                CE_ERROR("[Vulkan]: %s - %s", pCallbackData->pMessageIdName, pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                CE_WARN("[Vulkan]: %s - %s", pCallbackData->pMessageIdName, pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                CE_INFO("[Vulkan]: %s - %s", pCallbackData->pMessageIdName, pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                CE_TRACE("[Vulkan]: %s - %s", pCallbackData->pMessageIdName, pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                CE_TRACE("[Vulkan]: %s - %s", pCallbackData->pMessageIdName, pCallbackData->pMessage);
                break;
            default:
                CE_ERROR("[Vulkan]: %s - %s", pCallbackData->pMessageIdName, pCallbackData->pMessage);
                break;
        }
        
        return VK_FALSE;
    }
    
    void VulkanInstance::Init()
    {
        // General
        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = Application::Current()->GetSpecification().Name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "CeltEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;
        
        // Extensions
        const std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();
        CE_TRACE("Available Vulkan extensions:");
        for (const auto& extension : extensions) {
            CE_TRACE("\t%s", extension.extensionName);
        }

        uint32_t requiredExtensionCount = 0;
        const char** requiredExtensions;
        
        #if defined(CE_WINDOWS) || defined(CE_LINUX)
            requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
        #endif

        const char* myNewExtension = "VK_EXT_debug_utils"; 
        const char** newRequiredExtensions = static_cast<const char**>(malloc((requiredExtensionCount + 1) * sizeof(const char*)));

        for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
            newRequiredExtensions[i] = requiredExtensions[i];
        }

        newRequiredExtensions[requiredExtensionCount] = myNewExtension;
        ++requiredExtensionCount;
        requiredExtensions = newRequiredExtensions;
        
        for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
            bool found = false;

            for (const auto& layerProperties : extensions) {
                if (strcmp(requiredExtensions[i], layerProperties.extensionName) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                CE_ERROR("Vulkan extension %s could not be loaded!", requiredExtensions[i]);
            }
        }
        
        createInfo.enabledExtensionCount = requiredExtensionCount;
        createInfo.ppEnabledExtensionNames = requiredExtensions;

        // Layers
        std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
        CE_TRACE("Available Vulkan layers:");
        for (const auto& layer : layers) {
            CE_TRACE("\t%s", layer.layerName);
        }
        
        // TODO: Make this configurable
        const std::vector<const char*> requiredLayers = {
            #ifdef CE_DEBUG
            "VK_LAYER_KHRONOS_validation"
            #endif
        };

        for (const char* layerName : requiredLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : layers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                CE_ERROR("Vulkan layer %s could not be loaded!", layerName);
            }
        }

        createInfo.enabledLayerCount = requiredLayers.size();
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        
        // Creating Instance
        try {
            m_Instance = vk::createInstance(createInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create Vulkan instance: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created vulkan instance: succesful");

        // Dispatcher
        m_DynamicInstanceDispatcher = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
        
        // Create debug messenger
        #ifdef CE_DEBUG
        vk::DebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo;
        debugMessengerCreateInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
            | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
            | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
            | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
        debugMessengerCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
            | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
            | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
        debugMessengerCreateInfo.pfnUserCallback = debugCallback;
        debugMessengerCreateInfo.pUserData = nullptr; // Optional

        try {
            m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(debugMessengerCreateInfo, nullptr, m_DynamicInstanceDispatcher);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create debug messenger: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created debug messenger: succesful");
        #endif
    }

    void VulkanInstance::Shutdown()
    {
        #ifdef CE_DEBUG
        m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DynamicInstanceDispatcher);
        #endif
        m_Instance.destroy();
    }
}
