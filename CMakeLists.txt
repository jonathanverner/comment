CMAKE_MINIMUM_REQUIRED(VERSION 0.0)
PROJECT(annotPDF)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckLibraryExists)
INCLUDE(UsePkgConfig)
SET(PROJECT_NAME annotPDF)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

FIND_PACKAGE(Qt4)
FIND_PACKAGE(Poppler)
FIND_PACKAGE(LIBPODOFO REQUIRED)

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}
 ${QT_QTCORE_INCLUDE_DIR}
 ${QT_QTGUI_INCLUDE_DIR}
 ${QT_QTNETWORK_INCLUDE_DIR}
 ${POPPLER_INCLUDE_DIR}
 ${LIBPODOFO_INCL}
)

SET(UI_FILES
	propertyDialog.ui
)

QT4_WRAP_UI(UI_HDRS ${UI_FILES})

SET(ANNOT_SRC
  pdfPageItem.cpp
  myToolTip.cpp
  mainWindow.cpp
  testView.cpp
  pageBeginItem.cpp
  abstractTool.cpp
  textTool.cpp
  toolBox.cpp
  pdfScene.cpp
  sceneLayer.cpp
  pageNumberEdit.cpp
  pageView.cpp
  pdfUtil.cpp
  wordItem.cpp
  pageTextLayer.cpp
  hilightTool.cpp
  inlineTextTool.cpp
  teXjob.cpp
  renderTeX.cpp
  propertyTab.cpp
  searchBar.cpp
  search.cpp
  config.cpp
  hiliteItem.cpp
  util.cpp
)

SET(TEST_SRC
  testPoppler.cpp
  testPdfPageItem.cpp
  testPODOFO.cpp
  testAnnotRM.cpp
  testTeXRender.cpp
)



SET(LINK_LIBS
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARIES}
  ${QT_QTCORE_LIBRARIES}
  ${QT_QTGUI_LIBRARIES}
  ${POPPLER_LIBRARY}
  ${LIBPODOFO_LIB}
)

QT4_AUTOMOC(${ANNOT_SRC} ${TEST_SRC})
#QT4_WRAP_UI(QTBOARD_UI_HDRS ${QTBOARD_UI_FILES})

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )


IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
#if(Win32 AND NOT UNIX)
 # resource compilation for mingw
 ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/chess4skype_rc.o
 COMMAND i586-mingw32msvc-windres -I${CMAKE_CURRENT_SOURCE_DIR}
 -i${CMAKE_CURRENT_SOURCE_DIR}/chess4skype.rc
 -o ${CMAKE_CURRENT_BINARY_DIR}/chess4skype_rc.o)
 SET(chess4skype_SRCS ${chess4skype_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/chess4skype_rc.o)

#endif(Win32 AND NOT UNIX)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")


#ADD_EXECUTABLE(chessboard WIN32 ${chessboard_SRCS})
#TARGET_LINK_LIBRARIES(chessboard ${LINK_LIBS})
#INSTALL(TARGETS chessboard
#        DESTINATION "bin"
#)

#ADD_EXECUTABLE(chess4skype WIN32 ${chess4skype_SRCS})
#TARGET_LINK_LIBRARIES(chess4skype ${LINK_LIBS})
#INSTALL(TARGETS chess4skype
#        DESTINATION "bin"
#)


ADD_EXECUTABLE(comment comment.cpp ${ANNOT_SRC} ${UI_HDRS})
TARGET_LINK_LIBRARIES(comment ${LINK_LIBS})
INSTALL(TARGETS comment
	DESTINATION "bin"
)

ADD_EXECUTABLE(testPoppler ${ANNOT_SRC} testPoppler.cpp)
TARGET_LINK_LIBRARIES(testPoppler ${LINK_LIBS})

ADD_EXECUTABLE(testPdfPageItem ${ANNOT_SRC} testPdfPageItem.cpp)
TARGET_LINK_LIBRARIES(testPdfPageItem ${LINK_LIBS})


ADD_EXECUTABLE(testPODOFO testPODOFO.cpp)
TARGET_LINK_LIBRARIES(testPODOFO ${LINK_LIBS})

ADD_EXECUTABLE(annot_rm testAnnotRM.cpp)
TARGET_LINK_LIBRARIES(annot_rm ${LINK_LIBS})
INSTALL(TARGETS annot_rm
	DESTINATION "bin"
)


ADD_EXECUTABLE(testPageNumberEdit pageNumberEdit.cpp testPageNumberEdit.cpp config.cpp)
TARGET_LINK_LIBRARIES(testPageNumberEdit ${LINK_LIBS})

ADD_EXECUTABLE(testTeXRender testTeXRender.cpp renderTeX.cpp teXjob.cpp config.cpp)
TARGET_LINK_LIBRARIES(testTeXRender ${LINK_LIBS})


IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
ADD_DEFINITIONS(
#	-DQT_NO_DEBUG_OUTPUT
)
ELSE(CMAKE_SYSTEM_NAME MATCHES "Windows") 
ADD_DEFINITIONS(
        -DDEBUG
        -g
#	-DQT_NO_DEBUG_OUTPUT
)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

ADD_DEFINITIONS(
	${LIBPODOFO_CFLAGS}
)

FILE(GLOB docs doc/*)
FILE(GLOB images images/*png)
FILE(GLOB lang lang/*)


IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
SET( QT_DLL_DIR /home/joni/.wine/drive_c/Qt/4.3.2/bin )
  INSTALL(FILES
    ${QT_DLL_DIR}/QtCore4.dll
    ${QT_DLL_DIR}/QtGui4.dll
#    win/QtNetwork4.dll
    ${QT_DLL_DIR}/mingwm10.dll
    ${docs}
    DESTINATION "bin"
  )

  INSTALL(FILES
    ${pics}
    DESTINATION "bin/pics"
  )

  INSTALL(FILES
    ${lang}
    DESTINATION "bin/lang"
  )


ELSE(CMAKE_SYSTEM_NAME MATCHES "Windows")

INSTALL(FILES 
    ${docs}
    DESTINATION "share/doc/${PROJECT_NAME}"
)

INSTALL(FILES
    ${lang}
    DESTINATION "share/${PROJECT_NAME}/locale"
)


INSTALL(FILES
    ${images}
    DESTINATION "share/${PROJECT_NAME}/images"
)
#INSTALL(
#
#INSTALL(FILES
#    chess4skype.desktop
#    DESTINATION "share/applnk/Games/Board"
#)
#
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")





     


#INSTALL(PROGRAMS
#    findOkularXML.sh
#    DESTINATION "bin"
#)


  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Annotate PDF Files")
  SET(CPACK_PACKAGE_VENDOR "Jonathan Verner")
  SET(CPACK_PACKAGE_CONTACT "Jonathan Verner <jonathan.verner@matfyz.cz>")
  #  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/doc/License.txt")
  #SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/doc/README.txt")
  SET(CPACK_PACKAGE_VERSION_MAJOR "0")
  SET(CPACK_PACKAGE_VERSION_MINOR "0")
  SET(CPACK_PACKAGE_VERSION_PATCH "0")
  SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "annotPDF ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_SOURCE_PACKAGE_FILE_NAME "annotPDF-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  SET(CPACK_PACKAGE_EXECUTABLES 
       "annotPDF;Annotate PDF" 
  )

#  SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/installer.bmp")
#  SET(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/chess4skype.ico")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\jonathan.verner.matfyz.cz")
  SET(CPACK_NSIS_CONTACT "jonathan.verner@matfyz.cz")


  SET(DEBIAN_PACKAGE_DEPENDS "libqt4-core, libqt4-gui, libpoppler-qt")
  #  SET(DEBIAN_PACKAGE_SECTION "universe/net")
  SET(DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
  INCLUDE(CPack)
  IF(DEBIAN_FOUND)
    ADD_DEBIAN_TARGETS(annotPDF)
  ENDIF(DEBIAN_FOUND)


