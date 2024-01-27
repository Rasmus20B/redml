set(SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/main.cc
)

add_library(interpret)
target_sources(interpret
  PUBLIC
  FILE_SET CXX_MODULES FILES
  interpret/parser.cc
  interpret/repl.cc 
  interpret/tokenizer.cc 
  interpret/tokens.cc
  interpret/codegen.cc
  )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE interpret)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
