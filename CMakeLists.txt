CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

PROJECT(CodeCrypto)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0005 NEW)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

INCLUDE(AddFileDependencies)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckFunctionExists)
INCLUDE(FindPkgConfig)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")

#SET(VERSION_SUFFIX "")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
IF(VERSION_SUFFIX)
	SET(VERSION "${VERSION}-${VERSION_SUFFIX}")
ENDIF(VERSION_SUFFIX)

INCLUDE(DetectMachine)

LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)


# set compiler flags
SET(WERROR_FLAGS "-Wall -Werror=unused-function -Wno-sign-compare -Wno-strict-overflow")
OPTION(USE_WERROR "Add -werror to the build flags. Stops the build on warnings" OFF)
IF(${USE_WERROR})
   SET(WERROR_FLAGS "${WERROR_FLAGS} -Werror")
ENDIF()

# Due to a regression in gcc-4.8.X, we need to disable array-bounds check
IF (CMAKE_COMPILER_IS_GNUCXX AND ((CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL "4.8.0") OR (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.8.0") OR FABARIAGEST_BUILD_WIN32))
   SET(WERROR_FLAGS "${WERROR_FLAGS} -Wno-array-bounds")
ENDIF()

IF(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Setting build type to 'Release' as none was specified.")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
  		"MinSizeRel" "RelWithDebInfo")
ENDIF()

SET(CMAKE_C_FLAGS "${WERROR_FLAGS} ${CMAKE_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "-fno-exceptions ${WERROR_FLAGS} ${CMAKE_CXX_FLAGS}")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCodeCrypto_DEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCodeCrypto_DEBUG")


IF(WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
ELSE(WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DPIC")
ENDIF(WIN32)

# make sub-directories
ADD_SUBDIRECTORY(src)

#
# add distclean-target
#
ADD_CUSTOM_TARGET(distclean
			COMMAND make clean
			COMMAND rm -rf `find -name cmake_install.cmake` `find -name Makefile` `find -type d -name CMakeFiles` CMakeCache.txt)

#
# display configuration information
#

MESSAGE("\n"
"Installation Summary\n"
"--------------------\n"
"* Install Directory           : ${CMAKE_INSTALL_PREFIX}\n"
"* Build type                  : ${CMAKE_BUILD_TYPE}\n"
"* Platform                    : ${CMAKE_SYSTEM_PROCESSOR} (${Machine})\n"
)

MESSAGE(
"\n"
"-----------------------------------------------------------------\n"
"IMPORTANT:\n"
"after installing missing packages, remove CMakeCache.txt before\n"
"running cmake again!\n"
"-----------------------------------------------------------------\n"
"\n\n")

INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Diploma!")
SET(CPACK_PACKAGE_VENDOR "Codelinfoft Developers")
IF(FABARIAGEST_BUILD_APPLE)
	CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/README" "${CMAKE_BINARY_DIR}/README.txt" COPYONLY)
	CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/COPYING" "${CMAKE_BINARY_DIR}/COPYING.txt" COPYONLY)
	SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_BINARY_DIR}/README.txt")
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_BINARY_DIR}/COPYING.txt")
ELSE(FABARIAGEST_BUILD_APPLE)
	SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
ENDIF(FABARIAGEST_BUILD_APPLE)

SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")

IF(VERSION_SUFFIX)
	SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_SUFFIX}")
ENDIF(VERSION_SUFFIX)
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CodeCrypto")
IF(WIN32)
	SET(CPACK_GENERATOR NSIS)
	SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "
		!define MUI_ICON  \\\"${CMAKE_SOURCE_DIR}/src/images/CodeCrypto.ico\\\"
		!define MUI_UNIICON \\\"${CMAKE_SOURCE_DIR}/src/images/CodeCrypto.ico\\\" ")
	SET(CPACK_NSIS_INSTALLER_MUI_FINISHPAGE_RUN_CODE 
	"!define MUI_FINISHPAGE_RUN \\\"$INSTDIR\\\\CodeCrypto.exe\\\" "
	"!define MUI_FINISHPAGE_SHOWREADME \\\"$INSTDIR\\\\README.txt\\\" ")
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "CodeCrypto.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "CodeCrypto ${VERSION}")
	SET(CPACK_NSIS_HELP_LINK "http://www.codelinsoft.it")
	SET(CPACK_NSIS_URL_INFO_ABOUT "http://www.codelinsoft.it")
	SET(CPACK_NSIS_CONTACT "info@codelinsoft.it")
	SET(CPACK_PACKAGE_EXECUTABLES "CodeCrypto.exe;CodeCrypto")
	SET(CPACK_NSIS_MENU_LINKS "CodeCrypto.exe;CodeCrypto")
	SET(CPACK_CREATE_DESKTOP_LINKS "CodeCrypto.exe;CodeCrypto")
	set(CPACK_NSIS_CREATE_ICONS_EXTRA "
    	CreateShortCut \\\"$DESKTOP\\\\CodeCrypto.lnk\\\" \\\"$INSTDIR\\\\CodeCrypto.exe\\\" ")
	set(CPACK_NSIS_DELETE_ICONS_EXTRA "
    		Delete \\\"$DESKTOP\\\\CodeCrypto.lnk\\\"")
	SET(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}/cmake/nsis/FileAssociation.nsh")
	SET(CPACK_PACKAGE_FILE_NAME "CodeCrypto-${VERSION}-win32")
ELSE(WIN32)
	SET(CPACK_PACKAGE_EXECUTABLES "CodeCrypto" "CodeCrypto binary")
ENDIF(WIN32)

IF(WIN64)
	SET(CPACK_PACKAGE_FILE_NAME "CodeCrypto-${VERSION}-win64")
	SET(CPACK_NSIS_DEFINES "${CPACK_NSIS_DEFINES}
		InstallDir \\\"\\\$PROGRAMFILES64\\\\${CPACK_PACKAGE_INSTALL_DIRECTORY}\\\"")
ENDIF(WIN64)

SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "CodeCrypto-${VERSION}")
INCLUDE(CPack)

