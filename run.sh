cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make

cmake --build . --target hex
cmake --build . --target upload

cd ..
