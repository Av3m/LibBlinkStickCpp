set(INC_FILES BlinkStickDevice.h
	      BlinkStick.h
	      BlinkStickException.h
	      BlinkStickCommands.h
	      BlinkStickColor.h)

set(SRC_FILES BlinkStickDevice.cpp
	      BlinkStick.cpp
	      BlinkStickCommands.cpp
	      BlinkStickColor.cpp)


add_library(libblinkstick ${INC_FILES} ${SRC_FILES})

target_include_directories(libblinkstick PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(libblinkstick
    ${CONAN_LIBS_HIDAPI} ${Boost_LIBRARIES})

install(TARGETS libblinkstick EXPORT LibBlinkStickConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
    



