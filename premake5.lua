-- Check if the file or directory exists in this path
function Exists(file)
    local ok, err, code = os.rename(file, file)
    if not ok then
        if code == 13 then
            -- Permission denied, but it exists
            return true
        end
    else
        return true
    end

    return false
end

-- Remove the directory if it exists
function Remove_Directory(folder)
    if Exists(folder) then
        print(folder .. " exists and will be removed")
        if package.config:sub(1,1) == '\\' then
            os.execute('rd /s/q "'..folder..'"')
        end
    else
        print(folder .. " not exists")
    end
end

-- Remove the file if it exists
function Remove_File(file)
    if Exists(file) then
        print(file .. " exists and will be removed")
        os.remove(file)
    else
        print(file .. " not exists")
    end
end


-- Remove unnecessary files and directories
Remove_Directory(".vs")
Remove_Directory("bin")
Remove_Directory("bin-int")
Remove_File("RedMakeUp/RedMakeUp.vcxproj")
Remove_File("SandBox/SandBox.vcxproj")
Remove_File("RedMakeUp.sln")




workspace "RedMakeUp"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.archtecture}"

project "SandBox"
    location "SandBox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "RedMakeUp/vendors/spdlog/include",
        "RedMakeUp/src"
    }

    links
    {
        "RedMakeUp"
    }

    filter "system:Windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RMU_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RMU_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RMU_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "RMU_DIST"
        symbols "On"

project "RedMakeUp"
    location "RedMakeUp"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "stdafx.h"
    pchsource "%{prj.name}/src/stdafx.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendors/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:Windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RMU_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "RMU_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RMU_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "RMU_DIST"
        symbols "On"
