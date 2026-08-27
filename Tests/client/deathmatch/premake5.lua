project "Tests_ClientDeathmatch"
	language "C++"
	kind "ConsoleApp"
	targetname "Tests_ClientDeathmatch"
	targetdir(buildpath("tests"))

	vpaths {
		["Sources/*"] = {"**.cpp", "../main.cpp"},
		["*"] = "premake5.lua"
	}

	includedirs {
		"../../../Client/mods/deathmatch/headless",
		"../../../vendor/googletest/include"
	}

	links { "Client Deathmatch Headless", "gtest" }

	files {
		"premake5.lua",
		"../main.cpp",
		"**.cpp"
	}

	defines { "GTEST_HAS_PTHREAD=0" }

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
