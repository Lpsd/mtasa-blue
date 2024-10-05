project "CEGUITinyXML2Parser"
	language "C++"
	kind "SharedLib"
	targetname "CEGUITinyXML2Parser"
	targetdir(buildpath("mta"))

	includedirs {
		"../../include",
		"../../../freetype/include",
        "../../dependencies/pcre-8.12",
        "../../dependencies/tinyxml2-9.0.0",
        "../../dependencies/glm-0.9.4.5"
	}

    links {
        "glm-0.9.4.5",
        "CEGUI-0.8.7",
        "tinyxml2-9.0.0",
		"freetype", 
		"pcre-8.12",
		"dbghelp",
        "winmm",
        "d3d9",
    }

	defines {
		"CEGUITINYXML2PARSER_EXPORTS",
		"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
	}

	vpaths {
		["Headers/*"] = "../../include/CEGUI/XMLParserModules/TinyXML2/**.h",
		["Sources/*"] = "**.cpp",
		["Sources/*"] = "**.c",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"**.cpp",
		"**.c",
		"../../include/CEGUI/XMLParserModules/TinyXML2/**.h",
	}

	filter "architecture:x64"
		flags { "ExcludeFromBuild" }
	filter "system:not windows"
		flags { "ExcludeFromBuild" }

	filter {"system:windows"}
		linkoptions { "/ignore:4221" }
		disablewarnings { "4221" }

    configuration "Debug"
        links { "d3dx9d" }
    configuration "Release"
        links { "d3dx9" }
	 configuration {}
