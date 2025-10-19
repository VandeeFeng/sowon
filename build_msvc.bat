@echo off
rem launch this from msvc-enabled console

set COMMON_CXXFLAGS=/std:c++17 /O2 /FC /W4 /nologo /I thirdparty /I build
set SDL2_CXXFLAGS=%COMMON_CXXFLAGS% /I SDL2\include
set RGFW_CXXFLAGS=%COMMON_CXXFLAGS%
set COMMON_LIBS=Shell32.lib
set SDL2_LIBS=%COMMON_LIBS% SDL2\lib\x64\SDL2.lib SDL2\lib\x64\SDL2main.lib -SUBSYSTEM:windows
set RGFW_LIBS=%COMMON_LIBS% -SUBSYSTEM:windows /entry:mainCRTStartup

mkdir build
cl.exe %COMMON_CXXFLAGS% /Febuild\png2c src\png2c.c /link %COMMON_LIBS% -SUBSYSTEM:console
build\png2c.exe assets\digits.png digits > build\digits.h
build\png2c.exe assets\penger_walk_sheet.png penger > build\penger_walk_sheet.h
cl.exe %SDL2_CXXFLAGS% /Fesowon src/main.c /link %SDL2_LIBS%
cl.exe %RGFW_CXXFLAGS% /Fesowon_rgfw src/main_rgfw.c /link %RGFW_LIBS%
