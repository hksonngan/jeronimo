# @file CmakeList.txt
# @brief Project configuration file.
# @author Alexander Pinzon Fernandez
# @date 21/01/2011 10:04 a.m.
PROJECT(sjSkeletonizer)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

IF ( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )  
ENDIF()

IF    (CMAKE_COMPILER_IS_GNUCC)
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions")
ELSEIF(MSVC71 OR MSVC80)
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /EHs-")
ENDIF (CMAKE_COMPILER_IS_GNUCC)


FIND_PACKAGE(CGAL QUIET REQUIRED COMPONENTS Core Qt4)
INCLUDE( ${CGAL_USE_FILE} )

SET( QT_USE_QTXML    TRUE )
SET( QT_USE_QTMAIN   TRUE )
SET( QT_USE_QTSCRIPT  TRUE )
SET( QT_USE_QTOPENGL  TRUE )

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(TAUCS)
	
IF(QT4_FOUND)
	INCLUDE(${QT_USE_FILE})
	FIND_PACKAGE(QGLViewer REQUIRED)
ENDIF(QT4_FOUND)

IF (MSVC)
	# Use /FR to turn on IntelliSense
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FR")
ENDIF()

IF(CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)

	#qt4_wrap_ui( UI_FILES sjSkeletonizerUI.ui )
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
	INCLUDE(AddFileDependencies)
	INCLUDE_DIRECTORIES(${QGLVIEWER_INCLUDE_DIR})
	INCLUDE_DIRECTORIES(${TAUCS_INCLUDE_DIR})

	qt4_generate_moc( "${CMAKE_CURRENT_SOURCE_DIR}/sjViewer.h" ${CMAKE_CURRENT_BINARY_DIR}/sjViewer_moc.cpp )
	SET(FOO_MOC_FILE "${CMAKE_CURRENT_BINARY_DIR}/sjViewer_moc.cpp")
	add_file_dependencies( ${FOO_MOC_FILE} "${CMAKE_CURRENT_SOURCE_DIR}/sjViewer.h" )
	
	#SET(QtApp_RCC_SRCS sjViewer.qrc)

	#QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCC_SRCS})

	
	SET(sjSkeletonizer_SOURCE
		sjMain.cpp
		sjViewer.h
		sjViewer.cpp
		${FOO_MOC_FILE}
		sjConfig.h
		include/sjDataIO.h
		include/sjDataType.h
		src/sjDataIO.cpp
		include/sjLaplacianSmoothing.h
		src/sjLaplacianSmoothing.cpp
		include/sjUtils.h
		src/sjUtils.cpp
	)
	

   #ADD_EXECUTABLE ( sjSkeletonizer ${sjSkeletonizer_SOURCE} ${UI_FILES} ${QtApp_RCC_SRCS})
ADD_EXECUTABLE ( sjSkeletonizer ${sjSkeletonizer_SOURCE} )

  TARGET_LINK_LIBRARIES ( sjSkeletonizer 
	${QT_LIBRARIES} 
	${CGAL_LIBRARIES} 
	${CGAL_3RD_PARTY_LIBRARIES} 
	${QGLVIEWER_LIBRARIES} 
	${OPENGL_gl_LIBRARY} 
	${OPENGL_glu_LIBRARY} 
	${TAUCS_LIBRARIES_DIR}/libtaucs)

	add_to_cached_list( CGAL_EXECUTABLE_TARGETS qtviewer )

ELSE (CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)
    message(STATUS "THE REQUIRED LIBRARIES ARE NOT PRESENT.")
ENDIF (CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)

