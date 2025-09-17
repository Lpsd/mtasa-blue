project "WebView2"
	language "C++"
	kind "SharedLib"
	targetname "cwebview2"
	targetdir(buildpath("mta"))

	includedirs {
		".",
		"../../Shared/sdk",
		"../sdk",
		"../../vendor/sparsehash/src/windows"
	}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	links {
		"d3dx9.lib",
		"dxerr.lib",
		"d3d11.lib",
		"dcomp.lib",
		"dxgi.lib"
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

    local wv2ver = "1.0.3485.44"
    local wv2pkg = ("$(SolutionDir)\\packages\\Microsoft.Web.WebView2.%s"):format(wv2ver)

    filter { "system:windows", "platforms:x86" }
		architecture "x86"
		nuget { ("Microsoft.Web.WebView2:%s"):format(wv2ver) }
		vsprops { NugetRoot = wv2pkg }
		libdirs { "$(NugetRoot)\\build\\native\\x86" }
		includedirs { "$(NugetRoot)\\build\\native\\include" }
		links { "WebView2LoaderStatic" }
		buildoptions { "-Zm130" }

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
