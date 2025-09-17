project "Client SDK"
	language "C++"
	kind "StaticLib"
	targetname "sdk"

	vpaths {
		["Headers/*"] = { "**.h", "**.hpp" },
		["*"] = "premake5.lua"
	}

	includedirs {
		"."
	}

	files {
		"premake5.lua",
		"**.h",
		"**.hpp",
	}

	flags { "ExcludeFromBuild" }
