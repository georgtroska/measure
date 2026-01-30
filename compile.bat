cd build
cmake --build . --config Release 
copy *.rootmap Release\
copy *.pcm Release\
cd ..
