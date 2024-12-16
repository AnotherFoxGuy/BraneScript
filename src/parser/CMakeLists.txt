find_package(tree-sitter CONFIG REQUIRED)

add_library(parser STATIC 
documentParser.cpp
documentContext.cpp
)
target_link_libraries(parser PUBLIC tree-sitter::tree-sitter tree-sitter-branescript types)
