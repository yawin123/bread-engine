# CMakeLists.txt en la carpeta de STB

# Lista de archivos fuente de STB (puedes agregar más archivos si es necesario)
file(GLOB STB_SOURCE_FILES src/*.c)

# Genera una biblioteca de encabezados para STB
add_library(stb STATIC ${STB_SOURCE_FILES})

# Establece los directorios de inclusión para los encabezados de STB
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
