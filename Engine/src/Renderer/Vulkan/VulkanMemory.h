﻿#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    class VulkanMemory
    {
    public:
          static uint32_t FindMemoryIndex(VulkanDevice* deivce, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
              const vk::PhysicalDeviceMemoryProperties memProperties = deivce->GetPhysicalDevice().getMemoryProperties();
              for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                  if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                      return i;
                  }
              }
              CE_ERROR("Failed to find suitable gpu memory type!");
              return -1;
          }
    };
}
