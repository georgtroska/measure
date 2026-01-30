mkdir build
cd build
cmake -Wno-dev -G"Visual Studio 17 2022" -A x64 -Thost=x64 -DCMAKE_VERBOSE_MAKEFILE=ON ..\src\
cd ..