find_package(Doxygen REQUIRED dot)

set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_EXTRACT_ALL YES)

doxygen_add_docs(doxygen src test)

add_custom_target(docs
  COMMAND make
  DEPENDS doxygen
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex
)
