set(SOURCES
   main.cpp
   surface.cpp
   window.cpp)

add_executable(main ${SOURCES})
target_include_directories(main PUBLIC ${PROJECT_INCLUDE_DIR})
target_link_libraries(main PUBLIC SDL2)
target_link_libraries(main PUBLIC SDL2_image)