import os
import platform
import subprocess

def main():
    print("Installing system packages...")
    if platform.system() == "Windows":
        subprocess.call(["winget", "install", "Git.Git"])
    else:
        subprocess.call(["sudo", "apt", "install", "git", "-y"])
        subprocess.call(["sudo", "apt", "install", "build-essential", "-y"])
        subprocess.call(["sudo", "apt", "install", "make", "-y"])

    print("\nUpdating git submodules ...")
    subprocess.call(["git", "submodule", "update", "--recursive", "--remote"])
    subprocess.call(["git", "submodule", "update", "--init", "--recursive", "--remote"])

    print("Checking Vulkan SDK...")
    vulkanSDK = os.environ.get("VULKAN_SDK")
    if (vulkanSDK is None):
        print("You don't have the Vulkan SDK installed!")
        print("Please install VulkanSDK Version 1.3.231.1 or higher from https://vulkan.lunarg.com/sdk/home")
        print("Make sure to select glm and debug libs during setup!")
        print("Rerun the setup after the installation finishes and make sure the env variable are set correctly!")
        return
    else:
        print(f"\nLocated Vulkan SDK at {vulkanSDK}")

    print("\nRunning premake...")
    if platform.system() == "Windows":
        subprocess.call(["Scripts/Dependencies/premake-5.0.0-beta2-windows/premake5.exe", "vs2022"])
    else:
        subprocess.call(["Scripts/Dependencies/premake-5.0.0-beta2-linux/premake5", "gmake2"])

main()