SET(IxProject_DIR ${CMAKE_INSTALL_PREFIX}/IxProject CACHE STRING "settting IxProject_DIR path" FORCE)

#SET(IxProject_DIR "" CACHE PATH "Location of IxProject")

FIND_PATH(IxProject_INCLUDE_DIR IxProject.h
	${IxProject_DIR}/include
    "${IxProject_DIR}/Public Headers"
    "$ENV{IxProject_PATH}/Public Headers"
    $ENV{IxProject_PATH}
    $ENV{IxProject_DIR}/include
    $ENV{IxProject_DIR}
    /usr/local/include
	/usr/local/IxProject/include
    /usr/include
	/usr/IxProject/include
	/opt/include
    /opt/local/IxProject/include
    /opt/IxProject/include
    "C:/IxProject/include"
	"C:/Program Files/IxProject/include"
)
MACRO(FIND_IxProject_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
PATHS
	${IxProject_DIR}/lib
    $ENV{IxProject_PATH}
    $ENV{IxProject_DIR}/lib
    $ENV{IxProject_DIR}
    /usr/local/lib
	/usr/local/IxProject/lib
    /usr/lib
	/usr/IxProject/lib
	/opt/lib
    /opt/local/IxProject/lib
    /opt/IxProject/lib
    "C:/IxProject/lib"
	"C:/Program Files/IxProject/lib"
)
ENDMACRO(FIND_IxProject_LIBRARY LIBRARY LIBRARYNAME)

FIND_IxProject_LIBRARY(IxProject_LIBRARY IxProject)
FIND_IxProject_LIBRARY(IxProject_LIBRARY_DEBUG IxProjectd)

SET(IxProject_FOUND FALSE)
IF (IxProject_INCLUDE_DIR AND IxProject_LIBRARY)
   SET(IxProject_FOUND TRUE)
ENDIF (IxProject_INCLUDE_DIR AND IxProject_LIBRARY)
