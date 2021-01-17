project "lunasvg"
    language "C++"
    cppdialect "C++17"
    kind "SharedLib"
    targetname "lunasvg"
    targetdir(buildpath("mta"))
    floatingpoint "Fast"
    rtti "Off"

    defines { "LUNASVG_EXPORTS", "LUNASVG_SHARED", "_CRT_SECURE_NO_WARNINGS" }

    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.cpp",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files { 
        "premake5.lua",
        "**.cpp",
        "**.h",
        "**.c"
    }

    includedirs { 
        "3rdparty/stb", 
        "3rdparty/plutovg", 
        "3rdparty/software", 
        "source/geometry", 
        "source/graphics", 
        "source", 
        "include", 
        "contrib"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }
    filter "system:not windows"
        flags { "ExcludeFromBuild" }

    filter {"system:windows"}
        linkoptions { "/ignore:4221" }
        disablewarnings { "4221" }