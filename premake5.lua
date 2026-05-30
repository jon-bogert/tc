workspace "tc"
architecture "x64"
    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "core"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    targetname "tc"
    targetdir ("bin/".. outputdir)
    objdir ("%{prj.name}/int/" .. outputdir)
    cppdialect "C++17"
    staticruntime "Off"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.c",
        "%{prj.name}/**.hpp",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.inl"
    }

    includedirs
    {
        "%{prj.name}/include",
        "%{prj.name}/include/jed",
        "%{prj.name}/src",
        "%{prj.name}/ext"
    }

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
        "DATETIME_STATIC"
    }

    links
    {
        "Shell32"
    }

    libdirs "%{prj.name}/lib"

    filter "system:windows"
		systemversion "latest"
		defines { "WIN32" }

	filter "configurations:Debug"
		defines { "_DEBUG", "_CONSOLE" }
		symbols "On"
        links
        {
            "yaml-cpp-d"
        }

    filter "configurations:Release"
		defines { "NDEBUG", "_CONSOLE" }
		optimize "On"
        links
        {
            "yaml-cpp"
        }

