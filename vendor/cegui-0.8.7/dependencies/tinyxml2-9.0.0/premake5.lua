project "tinyxml2-9.0.0"
	language "C++"
	kind "SharedLib"
	targetname "tinyxml2-9.0.0"
	targetdir(buildpath("mta"))

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["Sources/*"] = "**.c",
		["*"] = "premake5.lua"
	}

	defines {
		"TINYXML2_EXPORT"
	}

	files {
		"premake5.lua",
		"**.cpp",
		"**.c",
		"**.h",
	}

	filter "architecture:x64"
		flags { "ExcludeFromBuild" }
	filter "system:not windows"
		flags { "ExcludeFromBuild" }

	filter {"system:windows"}
		linkoptions { "/ignore:4221" }
		disablewarnings { "4221" }