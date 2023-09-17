workspace "Celt-Engine"

architecture "x86_64"
configurations { "Debug", "Release" }

flags { "MultiProcessorCompile" }

include "Engine"
include "Editor"

include "Sandbox"
