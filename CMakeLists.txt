cmake_minimum_required(VERSION 3.0)
project(BreadEngine)

# Agrega la fuente de GLFW a tu proyecto
add_subdirectory(libs/glfw)

# Recopila automáticamente todos los archivos en src y sus subdirectorios
file(GLOB SOURCES src/*.cpp src/**/*.cpp)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Set Position Independent Code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Agrega la biblioteca BreadEngine
add_library(BreadEngine SHARED ${SOURCES})

# Establece los directorios de inclusión
target_include_directories(BreadEngine PUBLIC include/ libs/glfw/include/ libs/stb/include libs/glad/include)

# Agrega los archivos fuente de Glad directamente a tu objetivo BreadEngine
target_sources(BreadEngine PRIVATE libs/glad/src/glad.c)

# Agrega los archivos fuente de STB directamente a tu objetivo BreadEngine
target_sources(BreadEngine PRIVATE libs/stb/src/stb_vorbis.c)

# Define la macro GLFW_INCLUDE_NONE antes de incluir el archivo de encabezado de GLFW
add_definitions(-DGLFW_INCLUDE_NONE)

# Vincula GLFW estáticamente
target_link_libraries(BreadEngine PUBLIC glfw)
