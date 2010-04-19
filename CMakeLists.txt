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

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/comment.1.gz
	COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/comment.1 | gzip -9 > ${CMAKE_CURRENT_BINARY_DIR}/comment.1.gz)
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/annot_rm.1.gz
	COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/annot_rm.1 | gzip -9 > ${CMAKE_CURRENT_BINARY_DIR}/annot_rm.1.gz)
SET(manpages ${CMAKE_CURRENT_BINARY_DIR}/comment.1.gz ${CMAKE_CURRENT_BINARY_DIR}/annot_rm.1.gz)

ADD_CUSTOM_TARGET(man_pages ALL DEPENDS ${manpages})

ADD_EXECUTABLE(comment comment.cpp ${ANNOT_SRC} ${UI_HDRS})
TARGET_LINK_LIBRARIES(comment ${LINK_LIBS})
INSTALL(TARGETS comment
	DESTINATION "bin"
)

ADD_EXECUTABLE(annot_rm testAnnotRM.cpp)
TARGET_LINK_LIBRARIES(annot_rm ${LINK_LIBS})
INSTALL(TARGETS annot_rm
	DESTINATION "bin"
)


ADD_DEFINITIONS(
        -DDEBUG
        -g
#	-DQT_NO_DEBUG_OUTPUT
)

ADD_DEFINITIONS(
	${LIBPODOFO_CFLAGS}
)

FILE(GLOB docs doc/*)
FILE(GLOB images images/*png)
FILE(GLOB lang lang/*)

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

INSTALL(FILES
	${manpages}
	DESTINATION "share/man/man1"
)

#INSTALL(FILES
#    chess4skype.desktop
#    DESTINATION "share/applnk/Games/Board"
#)
#
