clear

mkdir -p build
cd build

mkdir -p linux
cd linux

mkdir -p out
cd out

mkdir -p shaders

glslc ../../../shaders/default.vert -o ./shaders/default.vert
glslc ../../../shaders/default.frag -o ./shaders/default.frag

cp -r ../../../textures ./

cd ..
mkdir -p cmake
cd cmake

cmake ../../../
make

mv new_osge_project ../out
cd ../out

cp ../../../game.config ./game.config
chmod +x ./new_osge_project

./new_osge_project
