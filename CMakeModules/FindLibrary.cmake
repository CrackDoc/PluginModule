SET(Library_DIR ${CMAKE_INSTALL_PREFIX}/Library CACHE STRING "settting Library_DIR path" FORCE)

#SET(Library_DIR "" CACHE PATH "Location of Library")

FIND_PATH(Library_INCLUDE_DIR LibraryExport.h
	${Library_DIR}/include
    "${Library_DIR}/Public Headers"
    "$ENV{Library_PATH}/Public Headers"
    $ENV{Library_PATH}
    $ENV{Library_DIR}/include
    $ENV{Library_DIR}
    /usr/local/include
	/usr/local/Library/include
    /usr/include
	/usr/Library/include
	/opt/include
    /opt/local/Library/include
    /opt/Library/include
    "C:/Library/include"
	"C:/Program Files/Library/include"
)
MACRO(FIND_Library_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
PATHS
	${Library_DIR}/lib
    $ENV{Library_PATH}
    $ENV{Library_DIR}/lib
    $ENV{Library_DIR}
    /usr/local/lib
	/usr/local/Library/lib
    /usr/lib
	/usr/Library/lib
	/opt/lib
    /opt/local/Library/lib
    /opt/Library/lib
    "C:/Library/lib"
	"C:/Program Files/Library/lib"
)
ENDMACRO(FIND_Library_LIBRARY LIBRARY LIBRARYNAME)

FIND_Library_LIBRARY(Library_LIBRARY Library)
FIND_Library_LIBRARY(Library_LIBRARY_DEBUG Libraryd)

SET(Library_FOUND FALSE)
IF (Library_INCLUDE_DIR AND Library_LIBRARY)
   SET(Library_FOUND TRUE)
ENDIF (Library_INCLUDE_DIR AND Library_LIBRARY)
