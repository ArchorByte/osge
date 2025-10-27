clear

mkdir -p build
cd build

mkdir -p linux
cd linux

mkdir -p out
cd out

mkdir -p shaders

glslc ../../../game/shaders/default.vert -o ./shaders/default.vert
glslc ../../../game/shaders/default.frag -o ./shaders/default.frag

cp -r ../../../game/textures ./

cd ..
mkdir -p cmake
cd cmake

cmake ../../../
make

mv new_osge_project ../out
cd ../out

cp ../../../config/game.config ./game.config
chmod +x ./new_osge_project

echo ""
./new_osge_project
