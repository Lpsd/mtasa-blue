project "Client Deathmatch Logic"
	language "C++"
	kind "StaticLib"
	targetname "client-deathmatch-logic"
	clangtidy "On"

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = {"README.md", "premake5.lua"}
	}

	files {
		"README.md",
		"premake5.lua",
		"**.h",
		"**.cpp"
	}

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
