@echo off
rem launch this from msvc-enabled console

set CXXFLAGS=/std:c++17 /O2 /FC /W4 /nologo
set INCLUDES=/I SDL2\include
set LIBS=SDL2\lib\x64\SDL2.lib SDL2\lib\x64\SDL2main.lib Shell32.lib

cl.exe %CXXFLAGS% /Fepng2c png2c.c /link Shell32.lib -SUBSYSTEM:console
png2c.exe digits.png digits > digits.h
png2c.exe penger_walk_sheet.png penger > penger_walk_sheet.h
cl.exe %CXXFLAGS% %INCLUDES% /Fesowon main.c /link %LIBS% -SUBSYSTEM:windows
cl.exe %CXXFLAGS% /Fesowon_rgfw main_rgfw.c /link Shell32.lib -SUBSYSTEM:windows /entry:mainCRTStartup
