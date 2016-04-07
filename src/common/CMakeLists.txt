set(PROJECT common)

file (GLOB HEADERS ./*.h)
file (GLOB SOURCES ./*.cpp)

include_directories(./ ../)

add_library (${PROJECT} STATIC ${SOURCES} ${HEADERS})
