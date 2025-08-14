@echo off
cls

if not exist "build" (mkdir "build")
cd build

if not exist "win64" (mkdir "win64")
cd win64

if not exist "out" (mkdir "out")
cd out

if not exist "shaders" (mkdir "shaders")

glslc ../../../shaders/default.vert -o ./shaders/default.vert
glslc ../../../shaders/default.frag -o ./shaders/default.frag

if not exist "textures" (mkdir "textures")
xcopy "../../../textures" "./textures" /E /I /H /Y

cd ..
if not exist "cmake" (mkdir "cmake")
cd cmake

cmake ../../../ -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe
C:/MinGW/bin/mingw32-make.exe

move new_osge_project.exe ../out/
cd ../out

copy "..\..\..\game.config" "./"
pause
start new_osge_project.exe

cd ../../../
pause
