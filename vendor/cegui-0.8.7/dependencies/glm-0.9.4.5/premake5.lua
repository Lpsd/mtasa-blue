project "glm-0.9.4.5"
	language "C++"
	kind "StaticLib"
	targetname "glm-0.9.4.5"
	targetdir(buildpath("mta"))

    includedirs {
        ".",
        "glm"
    }

	vpaths {
		["Headers/*"] = "**.h",
		["Headers/*"] = "**.hpp",
		["Sources/*"] = "**.cpp",
		["Sources/*"] = "**.c",
		["*"] = "premake5.lua"
	}

	defines {
		"GLM_EXPORTS"
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