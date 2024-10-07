project "GUI"
	language "C++"
	kind "SharedLib"
	targetname "cgui"
	targetdir(buildpath("mta"))

	filter "system:windows"
		includedirs { "../../vendor/sparsehash/src/windows" }

	filter {}
		includedirs {
            "../../Shared/sdk",
			"../sdk",
			"../../vendor/cegui-0.8.7/include",
            "../../vendor/cegui-0.8.7/dependencies/glm-0.9.4.5",
            "../../vendor/cegui-0.8.7/dependencies/pcre-8.12",
			"../../vendor/sparsehash/src/",
			"../../vendor/freetype/include",
		}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	defines {
		"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
	}

	links {
		"CEGUI-0.8.7",
		"CEGUICoreWindowRendererSet",
		"CEGUISILLYImageCodec",
		"CEGUIDirect3D9Renderer",
		"CEGUITinyXML2Parser",
        "glm-0.9.4.5",
        "pcre-8.12",
		"d3dx9.lib",
		"d3d9.lib",
		"dxerr.lib"
	}

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"*.h",
		"*.cpp"
    }
	
	filter "architecture:x64"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
        flags { "ExcludeFromBuild" }
        
	filter {"system:windows"}
        buildoptions { "-Zm180" }

	filter {"configurations:Debug"}
		links { "dbghelp" }
