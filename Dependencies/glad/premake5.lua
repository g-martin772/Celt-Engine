project "glad"
        kind "StaticLib"
        staticruntime "off"
        language "C"

        targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
        objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

        files {
            "glad.h",
            "glad.c",
            "khrplatform.h"
        }

        includedirs {
            "include/glad"
        }
    
        filter "system:windows"
            systemversion "latest"
            defines { 
                "_CRT_SECURE_NO_WARNINGS"
            }
         
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
    