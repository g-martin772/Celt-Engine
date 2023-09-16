project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    includedirs {
        "src"
    }
    
    links {
        "CeltEngine"
    }
    
    filter "system:windows"
        systemversion "latest"
        
        defines {
            "CE_WINDOWS"
        }

    filter "system:linux"
        defines {
            "CE_LINUX"
        }

    filter "configurations:Debug"
        defines { "CE_DEBUG" }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "CE_RELEASE" }
        runtime "Release"
        optimize "On"
