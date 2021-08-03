### HUFFMAN ###

This program is a simple tool useful for file compression. It is implemented
with Huffman coding algorithm and is able to compress any type of regular 
files such as text files, images etc. The program is able to generate
dictionary files or use external one to increase compression ratio.
The dictionary can be provided as an optional argument for compress
comand. To see the list of avaliable commands check -h option.

1. build:
- Create ./build/ directory and go to it
- Run "cmake -A x64 \.\."
- Run "cmake --build \. --config Release"
2. install (next step):
- Run "cmake -P cmake_install.cmake"
- The files installed will be placed in ./build/
3. testing (next step):
- Run "ctest -C Release"
- All tests are expected to be passed
4. running (next step):
- Go to ./bin/
- Run "./huffman.exe -h" to see the list of avaliable commands 
