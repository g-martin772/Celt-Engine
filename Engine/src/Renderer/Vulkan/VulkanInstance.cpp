#include "cepch.h"
#include "VulkanInstance.h"

#include "Core/Application.h"
#include "Core/Logger.h"

#if defined(CE_WINDOWS) || defined(CE_LINUX)
#include <gLFW/glfw3.h>
#endif

namespace CeltEngine
{
    void VulkanInstance::Init()
    {
        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = Application::Current()->GetSpecification().Name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "CeltEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        
#if defined(CE_WINDOWS) || defined(CE_LINUX)
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
#endif
        
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        try {
            m_Instance = vk::createInstance(createInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create Vulkan instance: {0}", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created vulkan instance: succesful");

        uint32 extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        CE_TRACE("Available Vulkan extensions:");
        for (const auto& extension : extensions) {
            CE_TRACE("\t%s", extension.extensionName);
        }
    }

    void VulkanInstance::Shutdown()
    {
        m_Instance.destroy();
    }
}
