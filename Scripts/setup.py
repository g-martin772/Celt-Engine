import os
import platform
import subprocess

os.chdir('./../')

print("\nUpdating git submodules ...")
subprocess.call(["git", "submodule", "update", "--recursive", "--remote"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive", "--remote"])

print("\nRunning premake...")
if platform.system() == "Windows":
    subprocess.call(["Scripts/Dependencies/premake-5.0.0-beta2-windows/premake5.exe", "vs2022"])
else:
    subprocess.call(["Scripts/Dependencies/premake-5.0.0-beta2-linux/premake5", "gmake2"])