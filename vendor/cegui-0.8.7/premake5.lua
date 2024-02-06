project "CEGUI087"
	language "C++"
	kind "StaticLib"
	targetname "CEGUI087"

	includedirs {
		"include"
	}

	vpaths {
		["Headers/*"] = "include/**.h",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"include/**.h",
	}

	excludes {
		"src/renderers/**",
		"src/pcre/ucptypetable.c",
		"src/pcre/ucptable.c",
		"src/pcre/ucp.c"
	}

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }
	
	filter "system:not windows"
		flags { "ExcludeFromBuild" }

	filter "configurations:Debug"
		libdirs "lib/Debug"
		links {
			"CEGUIBase-0_d",
			"CEGUIOpenGLRenderer-0_d",
			"CEGUIOgreRenderer-0_d",
			"CEGUIFreeImageImageCodec-0_d",
			"CEGUITinyXMLParser-0_d",
			"freetype_d",
			"jpeg_d",
			"libpng_d",
			"lua_d",
			"pcre_d",
			"SILLY_d",
			"tinyxml_d",
			"toluapp_d",
			"zlib_d"
		}
