project(Glad)


set(  HEADER_FILES 
    "include/glad/glad.h"
    "include/KHR/khrplatform.h"
    "src/glad.c"
)



add_library(${PROJECT_NAME} STATIC ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)