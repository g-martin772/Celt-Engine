workspace "Celt-Engine"

architecture "x86_64"
configurations { "Debug", "Release" }

flags { "MultiProcessorCompile" }

startproject "Sandbox"

include "Engine"
include "Editor"

include "Sandbox"

group "Dependencies"
    include "Dependencies/glfw"
group ""
