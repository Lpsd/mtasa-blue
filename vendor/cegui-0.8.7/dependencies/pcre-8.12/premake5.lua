project "pcre-8.12"
	language "C++"
	kind "SharedLib"
	targetname "pcre-8.12"
	targetdir(buildpath("mta"))

    includedirs {
        "."
    }

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.c",
		["*"] = "premake5.lua"
	}

	defines {
		"PCRE_DEFINITION",
        "HAVE_CONFIG_H"
	}

	files {
		"premake5.lua",
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