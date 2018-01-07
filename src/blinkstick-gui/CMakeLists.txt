set(INC_FILES blinkstick_gui.h
	      QMainWindow.h
	      StressStatus.h)
set(SRC_FILES blinkstick_gui.cpp
	      QMainWindow.cpp
	      StressStatus.cpp)

set(QT_QMAKE_EXECUTABLE ${CONAN_QT_ROOT}/bin/qmake)

find_package(Qt4 REQUIRED QtCore QtGui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(blinkstick_gui ${INC_FILES} ${SRC_FILES} resources.qrc)

target_include_directories(blinkstick_gui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ".")

target_link_libraries(blinkstick_gui
    libblinkstick Qt4::QtGui Qt4::QtCore)

if(WIN32) 
	if(MSVC)
		set_target_properties(blinkstick_gui PROPERTIES
			WIN32_EXECUTABLE YES
			LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
    endif()
endif()

install(TARGETS blinkstick_gui
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

foreach(i ${CONAN_BIN_DIRS_QT})
    install(DIRECTORY ${i}/ DESTINATION "bin" FILES_MATCHING PATTERN "*.dll")
    install(DIRECTORY ${i}/ DESTINATION "bin" FILES_MATCHING PATTERN "*.so")
endforeach()
    



