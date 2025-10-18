@echo off
rem launch this from msvc-enabled console

set CXXFLAGS=/std:c++17 /O2 /FC /W4 /nologo
set INCLUDES=/I SDL2\include /I thirdparty /I build
set LIBS=SDL2\lib\x64\SDL2.lib SDL2\lib\x64\SDL2main.lib Shell32.lib

mkdir build
cl.exe %CXXFLAGS% /Febuild\png2c src\png2c.c /link Shell32.lib -SUBSYSTEM:console
build\png2c.exe assets\digits.png digits > build\digits.h
build\png2c.exe assets\penger_walk_sheet.png penger > build\penger_walk_sheet.h
cl.exe %CXXFLAGS% %INCLUDES% /Fesowon src/main.c /link %LIBS% -SUBSYSTEM:windows
cl.exe %CXXFLAGS% /Fesowon_rgfw src/main_rgfw.c /link Shell32.lib -SUBSYSTEM:windows /entry:mainCRTStartup
