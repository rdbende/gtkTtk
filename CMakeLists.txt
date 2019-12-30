CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

## ===========================================================================
##  Project Information
## ===========================================================================
PROJECT ( gtkTtk )
## Package version information.
SET ( PKG_NAME          ${PROJECT_NAME} )
SET ( PKG_MAJOR_VERSION 0 )
SET ( PKG_MINOR_VERSION 4 )
SET ( PKG_BUILD_VERSION 0 )
SET ( PKG_VERSION
     "${PKG_MAJOR_VERSION}.${PKG_MINOR_VERSION}" )
SET ( PKG_NAME_VERSION  ${PKG_NAME}-${PKG_VERSION} )
SET ( PKG_VENDOR        "Géballin" )
SET ( PKG_HOME_DIR      ${PROJECT_SOURCE_DIR} )

SET ( LOAD_GTK_DYNAMICALLY ON )
SET ( NO_MSVCRT            ON  )
SET ( USE_GNOME            OFF )
SET ( VERBOSE              ON  )

IF ( VERBOSE )
# SET ( CMAKE_VERBOSE_MAKEFILE ON )
  SET ( GTK2_LOGGING_ENABLED   ON )
ENDIF ( VERBOSE )

SET ( PKG_SOURCES 
                  generic/gtkTtk_TreeView.cpp
                  generic/gtkTtk_Separator.cpp
                  generic/gtkTtk_Arrows.cpp )

SET ( PKG_SOURCES generic/gtkTtk_Init.cpp
                  generic/gtkTtk_Symbols.cpp
                  generic/gtkTtk_GtkApp.cpp
                  generic/gtkTtk_Utilities.cpp
                  generic/gtkTtk_Background.cpp
                  generic/gtkTtk_Button.cpp
                  generic/gtkTtk_CheckButton.cpp
                  generic/gtkTtk_RadioButton.cpp
                  generic/gtkTtk_ToolButton.cpp
                  generic/gtkTtk_Labelframe.cpp
                  generic/gtkTtk_Entry.cpp
                  generic/gtkTtk_Menubutton.cpp
                  generic/gtkTtk_Scrollbar.cpp
                  generic/gtkTtk_Scale.cpp
                  generic/gtkTtk_Progress.cpp
                  generic/gtkTtk_SizeGrip.cpp
                  generic/gtkTtk_Paned.cpp
                  generic/gtkTtk_Notebook.cpp
                  generic/gtkTtk_Combobox.cpp
)

MESSAGE ( STATUS "===========================================================" )
MESSAGE ( STATUS " Welcome to the ${PKG_NAME} ${PKG_VERSION} build system!" )
MESSAGE ( STATUS "  * Selected generator:  ${CMAKE_GENERATOR}" )
MESSAGE ( STATUS "  * Operating System ID: "
        ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR} )
MESSAGE ( STATUS "  * Installation Directory: " ${CMAKE_INSTALL_PREFIX} )
MESSAGE ( STATUS "===========================================================" )

## ===========================================================================
##  Locate Tcl/Tk
## ===========================================================================
MESSAGE ( STATUS "Searching for Tcl/Tk..." )
FIND_PACKAGE ( TCL REQUIRED )
FIND_PACKAGE ( TclStub REQUIRED )
MESSAGE ( STATUS "  TCL_TCLSH:               " ${TCL_TCLSH} )
MESSAGE ( STATUS "  TCL_INCLUDE_PATH:        " ${TCL_INCLUDE_PATH} )
MESSAGE ( STATUS "  TCL_STUB_LIBRARY:        " ${TCL_STUB_LIBRARY} )
MESSAGE ( STATUS "  TCL_LIBRARY:             " ${TCL_LIBRARY} )
MESSAGE ( STATUS "  TK_WISH:                 " ${TK_WISH} )
MESSAGE ( STATUS "  TK_INCLUDE_PATH:         " ${TK_INCLUDE_PATH} )
MESSAGE ( STATUS "  TK_STUB_LIBRARY:         " ${TK_STUB_LIBRARY} )
MESSAGE ( STATUS "  TTK_STUB_LIBRARY:        " ${TTK_STUB_LIBRARY} )
MESSAGE ( STATUS "  TK_LIBRARY:              " ${TK_LIBRARY} )
# STRING  ( REPLACE stub "" TCL_LIBRARY_STATIC   ${TCL_STUB_LIBRARY} )
# STRING  ( REPLACE stub "" TK_LIBRARY_STATIC    ${TK_STUB_LIBRARY} )
# MESSAGE ( STATUS "  TCL_LIBRARY_STATIC:      " ${TCL_LIBRARY_STATIC} )
# MESSAGE ( STATUS "  TK_LIBRARY_STATIC:       " ${TK_LIBRARY_STATIC} )

IF ( WIN32 AND NO_MSVCRT )
  STRING ( REPLACE /MD  /MT  CMAKE_C_FLAGS_RELEASE   ${CMAKE_C_FLAGS_RELEASE} )
  STRING ( REPLACE /MDd /MTd CMAKE_C_FLAGS_DEBUG     ${CMAKE_C_FLAGS_DEBUG} )
  STRING ( REPLACE /MD  /MT  CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  STRING ( REPLACE /MDd /MTd CMAKE_CXX_FLAGS_DEBUG   ${CMAKE_CXX_FLAGS_DEBUG} )
  SET (    CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /nodefaultlib:MSVCRT.LIB" )
  SET (    CMAKE_EXE_LINKER_FLAGS_RELEASE
        "${CMAKE_EXE_LINKER_FLAGS_RELEASE} LIBCMT.LIB" )
  SET (    CMAKE_EXE_LINKER_FLAGS_DEBUG
        "${CMAKE_EXE_LINKER_FLAGS_DEBUG} LIBCMTD.LIB" )
  SET (    CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} /nodefaultlib:MSVCRT.LIB" )
  SET (    CMAKE_SHARED_LINKER_FLAGS_RELEASE
        "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} LIBCMT.LIB" )
  SET (    CMAKE_SHARED_LINKER_FLAGS_DEBUG
        "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}
         /nodefaultlib:LIBCMT.LIB LIBCMTD.LIB" )
  ADD_DEFINITIONS ( -D_STATIC_CPPLIB )
  ADD_DEFINITIONS ( -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1 )
  ADD_DEFINITIONS ( -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1 )
ENDIF ( WIN32 AND NO_MSVCRT )

IF ( WIN32 )
  ADD_DEFINITIONS ( -DWIN32 )
  ADD_DEFINITIONS ( -D_WIN32 )
  ADD_DEFINITIONS ( -D_WINDOWS )
  ADD_DEFINITIONS ( -D__WIN32__ )
  SET ( CMAKE_SHARED_LIBRARY_PREFIX lib )
ENDIF ( WIN32 )

IF ( LOAD_GTK_DYNAMICALLY )
  MESSAGE ( STATUS "GTK+ will be loaded dynamically!" )
  ADD_DEFINITIONS   ( -DGTKTTK_LOAD_GTK_DYNAMICALLY )
ENDIF ( LOAD_GTK_DYNAMICALLY )
IF ( CMAKE_BUILD_TYPE STREQUAL Debug )
  ADD_DEFINITIONS   ( -DGTKTTK_SYNCHRONIZE )
  ADD_DEFINITIONS   ( -DGTKTTK_VERBOSE_XERROR_HANDLER )
ENDIF ( CMAKE_BUILD_TYPE STREQUAL Debug )
ADD_DEFINITIONS     ( -DGTKTTK_INSTALL_XERROR_HANDLER )
ADD_DEFINITIONS     ( -DUSE_TCL_STUBS )
ADD_DEFINITIONS     ( -DUSE_TK_STUBS )
ADD_DEFINITIONS     ( -DUSE_TTK_STUBS )
ADD_DEFINITIONS     ( -DPACKAGE_NAME="${PKG_NAME}" )
ADD_DEFINITIONS     ( -DPACKAGE_VERSION="${PKG_VERSION}" )
INCLUDE_DIRECTORIES ( ${TCL_INCLUDE_PATH} )
INCLUDE_DIRECTORIES ( ${TK_INCLUDE_PATH} )
INCLUDE_DIRECTORIES ( ${TK_INCLUDE_PATH}/tk-private/generic/ttk )
INCLUDE_DIRECTORIES ( ${PROJECT_SOURCE_DIR}/generic )

## ===========================================================================
##  Locate GNOME2...
## ===========================================================================
IF ( USE_GNOME )
  MESSAGE ( STATUS "Searching for GNOME..." )
  INCLUDE ( cmake/Modules/FindGNOME2.cmake OPTIONAL )
  IF ( GNOME_FOUND )
      MESSAGE ( STATUS "  GNOME Found!" )
      ADD_DEFINITIONS     ( -DGTKTTK_ENABLE_GNOME )
      INCLUDE_DIRECTORIES ( ${GNOME_INCLUDE_DIR} )
      LINK_LIBRARIES      ( ${GNOME_LIBRARIES} )
  ENDIF ( GNOME_FOUND )
  MESSAGE ( STATUS "  GNOME_INCLUDE_DIR:" )
  FOREACH ( path ${GNOME_INCLUDE_DIR} )
    MESSAGE ( STATUS "   + ${path}" )
  ENDFOREACH ( path )
  MESSAGE ( STATUS "  GNOME_LIBRARIES:" )
  FOREACH ( path ${GNOME_LIBRARIES} )
    MESSAGE ( STATUS "   + ${path}" )
  ENDFOREACH ( path )
ENDIF ( USE_GNOME )

## ===========================================================================
##  Locate GTK...
## ===========================================================================
MESSAGE ( STATUS "Searching for GTK2..." )
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK "gtk+-2.0")
if (GTK_FOUND)
  LINK_LIBRARIES( ${GTK_LIBRARIES})
  add_definitions(${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})

MESSAGE ( STATUS "  GTK 2.x Found!" )
endif()

## ===========================================================================
##  Target: gtkTtk_Symbols.[h|cpp]
## ===========================================================================
ADD_CUSTOM_COMMAND ( OUTPUT ${PROJECT_SOURCE_DIR}/generic/gtkTtk_Symbols.cpp
                            ${PROJECT_SOURCE_DIR}/generic/gtkTtk_Symbols.h
    DEPENDS library/GenarateSymbols.tcl
            generic/gtkTtk_Init.cpp
            generic/gtkTtk_GtkApp.cpp
            generic/gtkTtk_Utilities.h
            generic/gtkTtk_Utilities.cpp
    COMMAND ${TCL_TCLSH} GenarateSymbols.tcl
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/library )
SET_SOURCE_FILES_PROPERTIES ( ${PROJECT_SOURCE_DIR}/generic/gtkTtk_Symbols.cpp
                              PROPERTIES GENERATED true )
SET_SOURCE_FILES_PROPERTIES ( ${PROJECT_SOURCE_DIR}/generic/gtkTtk_Symbols.h
                              PROPERTIES GENERATED true )

SET ( PKG_TARGET_LIB_NAME ${PKG_NAME}${PKG_VERSION} )
## ===========================================================================
##  Target: gtkTtk shared library
## ===========================================================================
MESSAGE ( STATUS "===========================================================" )
MESSAGE ( STATUS "   ++++ Shared Library: ${PKG_TARGET_LIB_NAME} ++++" )
MESSAGE ( STATUS "===========================================================" )
ADD_LIBRARY ( ${PKG_TARGET_LIB_NAME} SHARED ${PKG_SOURCES} )
TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TCL_STUB_LIBRARY} )
TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TK_STUB_LIBRARY} )
IF ( TTK_STUB_LIBRARY )
  TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TTK_STUB_LIBRARY} )
ENDIF ( TTK_STUB_LIBRARY )
# IF ( TCL_LIBRARY_STATIC )
#   TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TCL_LIBRARY_STATIC} )
# ENDIF ( TCL_LIBRARY_STATIC )
# IF ( TK_LIBRARY_STATIC )
#   TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TK_LIBRARY_STATIC} )
# ENDIF ( TK_LIBRARY_STATIC )
IF ( LOAD_GTK_DYNAMICALLY )
  IF ( GTK2_GMODULE_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_GMODULE_LIBRARY} )
  ELSE ( GTK2_GMODULE_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_LIBRARIES} )
  ENDIF ( GTK2_GMODULE_LIBRARY )
  IF ( UNIX )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} dl )
  ENDIF ( UNIX )
ELSE ( LOAD_GTK_DYNAMICALLY )
  TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_LIBRARIES} )
  IF ( GTK2_gdk_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_gdk_LIBRARY} )
  ENDIF ( GTK2_gdk_LIBRARY )
  IF ( GTK2_gobject_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_gobject_LIBRARY} )
  ENDIF ( GTK2_gobject_LIBRARY )
  IF ( UNIX )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} gdk_pixbuf_xlib-2.0 )
  ENDIF ( UNIX )
ENDIF ( LOAD_GTK_DYNAMICALLY )

FILE ( WRITE library/pkgIndex.tcl 
"package ifneeded ttk::theme::gtkTtk ${PKG_VERSION} \\
  [list load [file join $dir lib${PKG_NAME}${PKG_VERSION}[info sharedlibextension]] gtkTtk]" )

INSTALL ( TARGETS ${PKG_TARGET_LIB_NAME}
          RUNTIME DESTINATION ${PKG_TARGET_LIB_NAME}
          LIBRARY DESTINATION ${PKG_TARGET_LIB_NAME}
          ARCHIVE DESTINATION ${PKG_TARGET_LIB_NAME} )
INSTALL ( FILES library/pkgIndex.tcl library/gtkTtk.tcl
          DESTINATION ${PKG_TARGET_LIB_NAME} )

ENABLE_TESTING ( )
ADD_TEST ( demo   ${TK_WISH} ${PROJECT_SOURCE_DIR}/demos/demo.tcl )
ADD_TEST ( styles ${TK_WISH} ${PROJECT_SOURCE_DIR}/demos/WidgetStates.tcl )
