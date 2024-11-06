-- Updated premake5.lua

workspace "Neko"
   configurations { "Debug", "Release" }

project "NekoEngine"
   kind "ConsoleApp"
   language "C"
   files { "**.c", "include/**.h" }
   
   includedirs { "SDL/include" } -- Path to SDL include directory

   filter "configurations:Debug"
      symbols "On"

   filter "configurations:Release"
      optimize "On"

   filter {}  -- Reset filters

   -- Add compiler flags for C99
   filter { "language:C", "toolset:gcc or clang" }
      buildoptions { "-std=c99" }

   filter { "language:C", "toolset:msc*" }
      buildoptions { "/std:c99" }

   filter { "system:windows", "architecture:x86" }
      libdirs { "SDL/lib/x86" } -- Path to SDL 32-bit lib directory
      links { "SDL2", "SDL2main" } -- Assuming SDL2 library on Windows, adjust as needed for your platform
  
   filter { "system:windows", "architecture:x86_64" }
      libdirs { "SDL/lib/x64" } -- Path to SDL 64-bit lib directory
      links { "SDL2", "SDL2main" } -- Assuming SDL2 library on Windows, adjust as needed for your platform
  
   filter "system:windows"
      libdirs { "SDL/lib/x64", "SDL/lib/x86" } -- Add other library directories if needed
