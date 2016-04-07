set(PROJECT mainapp)

file (GLOB HEADERS ./*.h)
file (GLOB SOURCES ./*.cpp)

include_directories(./ ../)

set(LIBRARIES common series)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})
add_dependencies(${PROJECT} ${LIBRARIES} stdlib libc )

install (TARGETS ${PROJECT} DESTINATION bin)
