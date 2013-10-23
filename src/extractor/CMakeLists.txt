file(GLOB header *.h)
file(GLOB source *.cpp)

add_library(extractor SHARED ${header} ${source})