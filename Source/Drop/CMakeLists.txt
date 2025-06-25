add_subdirectory(Core)

add_library(Drop STATIC
    "Drop.hpp"
    "Drop.cpp"
)

target_sources(Drop
    PRIVATE
   
)

target_link_libraries(Drop
    PUBLIC
    Core
)