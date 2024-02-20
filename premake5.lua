workspace "octree"
	architecture "x86_64"
	configurations { "Debug", "Release" }

project "octree"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	files {
		"src/**.cpp",
		"include/**.h",
	}

	includedirs {
		"include",
	}

	targetdir "bin"
	objdir "obj/%{cfg.buildcfg}"

	filter "configurations:Debug"
		ignoredefaultlibraries { "MSVCRT" }
		targetname "%{prj.name}d"
		optimize "off"
		symbols "on"

	filter "configurations:Release"
		optimize "on"
		symbols "off"
