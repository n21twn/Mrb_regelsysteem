cmake_minimum_required(VERSION 3.16)
project(SerialBoostReceiver LANGUAGES CXX)

# Zorg dat vcpkg correct gekoppeld wordt (alleen nodig als je CMake GUI gebruikt)
# Anders geef je dit mee in de command line met -DCMAKE_TOOLCHAIN_FILE=...
# set(CMAKE_TOOLCHAIN_FILE "C:/Users/nickt/OneDrive/Documenten/PlatformIO/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Stel je standaard C++ versie in
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vind Boost via vcpkg
find_package(Boost REQUIRED COMPONENTS asio)

# Voeg je uitvoerbare bestand toe
add_executable(serial_receiver main.cpp)

# Link Boost libraries
target_link_libraries(serial_receiver PRIVATE Boost::asio)
