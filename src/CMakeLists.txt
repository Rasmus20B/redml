set(SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/main.cc
	${CMAKE_SOURCE_DIR}/src/tokenizer.cc
	${CMAKE_SOURCE_DIR}/src/repl.cc
)
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
target_link_libraries(${PROJECT_NAME})
