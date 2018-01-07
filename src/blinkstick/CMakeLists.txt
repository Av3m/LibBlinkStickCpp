set(INC_FILES blinkstick_demo.h)
set(SRC_FILES blinkstick_demo.cpp)


add_executable(blinkstick ${INC_FILES} ${SRC_FILES})

target_include_directories(blinkstick PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ".")

target_link_libraries(blinkstick
    libblinkstick)

install(TARGETS blinkstick
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

foreach(i ${CONAN_BIN_DIRS_HIDAPI})
    install(DIRECTORY ${i} DESTINATION "bin" FILES_MATCHING PATTERN "*.dll")
    install(DIRECTORY ${i} DESTINATION "bin" FILES_MATCHING PATTERN "*.so")
endforeach()
    



