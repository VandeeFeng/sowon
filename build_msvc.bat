@echo off
rem launch this from msvc-enabled console

set COMMON_CXXFLAGS=/std:c++17 /O2 /FC /W4 /nologo /I thirdparty /I build
set RGFW_CXXFLAGS=%COMMON_CXXFLAGS%
set COMMON_LIBS=Shell32.lib
set RGFW_LIBS=%COMMON_LIBS% -SUBSYSTEM:windows /entry:mainCRTStartup

mkdir build
cl.exe %COMMON_CXXFLAGS% /Febuild\png2c src\png2c.c /link %COMMON_LIBS% -SUBSYSTEM:console
build\png2c.exe assets\digits.png digits > build\digits.h
build\png2c.exe assets\penger_walk_sheet.png penger > build\penger_walk_sheet.h
cl.exe %RGFW_CXXFLAGS% /Fesowon src/main.c /link %RGFW_LIBS%
