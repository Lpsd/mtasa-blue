project "lanes"
	language "C++"
	cppdialect "C++17"
	kind "StaticLib"
	targetname "lanes"
	targetdir(buildpath("mta"))

	vpaths {
        ["Sources"] = "src/*.c",
		["Headers"] = "src/*.h",
		["*"] = "premake5.lua"
	}

	files { 
		"premake5.lua",
		"src/*.c",
		"src/*.h"
	}

	includedirs { 
		"src",
        "../lua/src"
	}

    postbuildcommands {
        "copy \"%{wks.location}..\\vendor\\lanes\\src\\lanes.lua\" \"" .. buildpath(".") .. "mta\\lanes.lua\"",
    }

    filter "system:linux"
        defines { "_GNU_SOURCE" }
