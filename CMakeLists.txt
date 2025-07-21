# -G "MinGW Makefiles"
cmake_minimum_required(VERSION 3.10)

project(PEBPARSE)

add_executable(PEBParse src/PEBParsing_F.c)
add_executable(debugging src/debugging_F.c)
add_executable(Modules src/Modules_F.c)
add_executable(ProcessInformation src/ProcessInformation_F.c)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)

set(CMAKE_FIND_ROOT_PATH /usr/bin/)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)