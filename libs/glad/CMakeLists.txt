# CMakeLists.txt en la carpeta de Glad

# Archivos fuente de Glad (los archivos .c o .cpp)
set(GLAD_SOURCE_FILES src/glad.c) 

# Genera una biblioteca estática para Glad
add_library(glad STATIC ${GLAD_SOURCE_FILES})

# Establece los directorios de inclusión para los encabezados de Glad
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
