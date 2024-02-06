project "GUINew"
	language "C++"
	kind "SharedLib"
	targetname "cguinew"
	targetdir(buildpath("mta"))

	filter "system:windows"
		includedirs { "../../vendor/sparsehash/src/windows" }

	filter {}
		includedirs {
			"../../Shared/sdk",
			"../sdk",
			"../../vendor/cegui-0.8.7/include",
			"../../vendor/sparsehash/src/"
		}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	links {
		"CEGUI087",
		"d3dx9.lib",
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
	
	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
