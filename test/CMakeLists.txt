file(GLOB header *.h)
file(GLOB source *.cpp)

include_directories($ENV{HOME}/usr/include)
link_directories($ENV{HOME}/usr/Linux/64/gcc4/lib)

add_executable(extract_integer_grid ${source} ${header})
target_link_libraries(extract_integer_grid
lapack
zjucad-ptree
)
