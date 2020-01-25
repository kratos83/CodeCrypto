project(CodeCrypto)

cmake_minimum_required(VERSION 2.6)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules )

if(WIN32)
SET( CMAKE_CXX_FLAGS  "-fexceptions -std=c++11 -mwindows" )
ELSE()
SET( CMAKE_CXX_FLAGS  "-fexceptions -std=c++11" )
ENDIF()

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} 
		    ${Qt5Core_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} 
		    ${Qt5PrintSupport_INCLUDE_DIRS})
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)

set(CodeCrypto "CodeCrypto")

set(CodeCrypto_includes
${CMAKE_BINARY_DIR}/src
)

include_directories(${CodeCrypto_includes})

set(CodeCrypto_moc_hdrs 
	mainwindow.h
)

qt5_wrap_cpp(CodeCrypto_mocs ${CodeCrypto_moc_hdrs})

set(CodeCrypto_uis 
	ui/mainwindow.ui
)


qt5_wrap_ui(CodeCrypto_ui_hdrs ${CodeCrypto_uis})

set(CodeCrypto_srcs 
	mainwindow.cpp
	main.cpp
)


qt5_add_resources(RESOURCE_FILES images.qrc) 


IF(WIN32)
	SET(WINRC "${CMAKE_CURRENT_BINARY_DIR}/CodeCrypto.obj")
	ADD_DEFINITIONS(-D__USE_MINGW_ANSI_STDIO=1)
	ADD_CUSTOM_COMMAND(OUTPUT ${WINRC}
						COMMAND ${WINDRES}
							-I${CMAKE_CURRENT_SOURCE_DIR}
							-o${WINRC}
							-i${CMAKE_CURRENT_BINARY_DIR}/winicon.rc
						DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/winicon.rc)

ENDIF(WIN32)

LINK_LIBRARIES(-lpthread)

if (APPLE)
    set(SOURCE_FILES ${SOURCE_FILES} images/CodeCrypto.icns)
    set_source_files_properties(images/CodeCrypto.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    ADD_EXECUTABLE(CodeCrypto MACOSX_BUNDLE ${CodeCrypto_srcs} ${CodeCrypto_mocs} ${CodeCrypto_ui_hdrs} ${RESOURCE_FILES} ${WINRC} ${SOURCE_FILES})
else()
ADD_EXECUTABLE(CodeCrypto ${CodeCrypto_srcs} ${CodeCrypto_mocs} ${CodeCrypto_ui_hdrs} ${RESOURCE_FILES} ${WINRC} ${SOURCE_FILES})
endif()


target_link_libraries( CodeCrypto
  Qt5::Widgets Qt5::Gui 
  Qt5::Sql Qt5::Core 
  Qt5::PrintSupport 
)



IF(WIN32)
    INSTALL(TARGETS CodeCrypto RUNTIME DESTINATION .)
	INSTALL(FILES
		"${MINGW_PREFIX}/bin/Qt5Core.dll"
		"${MINGW_PREFIX}/bin/Qt5Gui.dll"
		"${MINGW_PREFIX}/bin/Qt5Svg.dll"
		"${MINGW_PREFIX}/bin/Qt5Sql.dll"
		"${MINGW_PREFIX}/bin/Qt5Widgets.dll"
		"${MINGW_PREFIX}/bin/Qt5PrintSupport.dll"
		"${MINGW_PREFIX}/bin/iconv.dll"
        "${MINGW_PREFIX}/bin/libbz2-1.dll"
        "${MINGW_PREFIX}/bin/libfreetype-6.dll"
        "${MINGW_PREFIX}/bin/libglib-2.0-0.dll"
        "${MINGW_PREFIX}/bin/libharfbuzz-0.dll"
        "${MINGW_PREFIX}/bin/libintl-8.dll"
        "${MINGW_PREFIX}/bin/libjasper-4.dll"
        "${MINGW_PREFIX}/bin/libjpeg-62.dll"
        "${MINGW_PREFIX}/bin/libpcre-1.dll"
        "${MINGW_PREFIX}/bin/libpcre2-16-0.dll"
        "${MINGW_PREFIX}/bin/libpng16-16.dll"
        "${MINGW_PREFIX}/bin/libstdc++-6.dll"
        "${MINGW_PREFIX}/bin/libtiff-5.dll"
        "${MINGW_PREFIX}/bin/libwebp-7.dll"
        "${MINGW_PREFIX}/bin/libwebpdemux-2.dll"
        "${MINGW_PREFIX}/bin/libwinpthread-1.dll"
        "${MINGW_PREFIX}/bin/zlib1.dll"
		"../README.txt"
		DESTINATION .
		)
    INSTALL(FILES 
        "win/gpg.exe"
        "win/iconv.dll"
        DESTINATION ./win)
IF(WIN64)
    INSTALL(FILES 
        "${MINGW_PREFIX}/bin/libgcc_s_seh-1.dll"
        DESTINATION . )
ELSE ()
    INSTALL(FILES 
        "${MINGW_PREFIX}/bin/libgcc_s_sjlj-1.dll"
        DESTINATION . )
ENDIF(WIN64)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt5/plugins/sqldrivers/qsqlite.dll" DESTINATION ./sqldrivers
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt5/plugins/platforms/qminimal.dll" 
		"${MINGW_PREFIX}/lib/qt5/plugins/platforms/qwindows.dll"
		DESTINATION ./platforms
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt5/plugins/audio/qtaudio_windows.dll" 
		DESTINATION ./audio
		)
	INSTALL(FILES
		"${MINGW_PREFIX}/lib/qt5/plugins/imageformats/qgif.dll"
		"${MINGW_PREFIX}/lib/qt5/plugins/imageformats/qico.dll"
		"${MINGW_PREFIX}/lib/qt5/plugins/imageformats/qjpeg.dll"
		"${MINGW_PREFIX}/lib/qt5/plugins/imageformats/qsvg.dll"
		DESTINATION ./imageformats
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt5/plugins/printsupport/windowsprintersupport.dll" 
		DESTINATION ./printsupport
		)
    INSTALL(FILES
        "${MINGW_PREFIX}/lib/qt5/plugins/styles/qwindowsvistastyle.dll" 
		DESTINATION ./styles
		)
ELSE()
    INSTALL( TARGETS CodeCrypto RUNTIME DESTINATION /opt/CodeCrypto)
    INSTALL( FILES "images/CodeCrypto.png" DESTINATION /opt/CodeCrypto)
endif()
