# @file CmakeList.txt
# @brief Project configuration file.
# @author Alexander Pinzon Fernandez
# @date 21/01/2011 10:04 a.m.
PROJECT(sjSkeletonizer)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

IF(WIN32)
	# visual studio specific section
	# controls inclusion of header files in solution
	# not really pretty but working
	
	SET(VISUAL_STUDIO_INCLUDE_HEADERS_IN_SOLUTION TRUE)
	
	# list of extensions added to the header folder
	SET(VISUAL_STUDIO_INCLUDE_HEADERS_EXTENSIONS_LIST *.h *.hpp *.hxx *.inl)
	
	
	IF(VISUAL_STUDIO_INCLUDE_HEADERS_IN_SOLUTION)
		MACRO ( GatherProjectFiles ProjectName ProjectDir ProjectSources )
		 
			# get the file lists
			SET ( Dir ${CMAKE_HOME_DIRECTORY}/${ProjectDir} )
			
			FOREACH(var ${VISUAL_STUDIO_INCLUDE_HEADERS_EXTENSIONS_LIST})
				LIST(APPEND VS_EXTLIST_INCLUSION ${Dir}/${var})
			ENDFOREACH(var)

			FILE ( GLOB_RECURSE DirSources ${VS_EXTLIST_INCLUSION})
			
		   SET ( "${ProjectSources}" ${DirSources} )
		ENDMACRO ( GatherProjectFiles )
		
	ELSE(VISUAL_STUDIO_INCLUDE_HEADERS_IN_SOLUTION)
	
		MACRO ( GatherProjectFiles ProjectName ProjectDir ProjectSources )
		ENDMACRO ( GatherProjectFiles )
		
	ENDIF(VISUAL_STUDIO_INCLUDE_HEADERS_IN_SOLUTION)
ENDIF(WIN32)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

IF ( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )  
ENDIF()

SET(BUILD_SHARED_LIBS ON)

SET(BINARIES_RELATIVE_PATH /binaries/bin/)

IF (WIN32)
	SET(LIBRARIES_RELATIVE_PATH /binaries/bin/)
ELSE(WIN32)
	SET(LIBRARIES_RELATIVE_PATH /binaries/lib/)
ENDIF(WIN32)

IF(WIN32)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}${BINARIES_RELATIVE_PATH})
ELSE(WIN32)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}${LIBRARIES_RELATIVE_PATH})
ENDIF(WIN32)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}${BINARIES_RELATIVE_PATH})


IF    (CMAKE_COMPILER_IS_GNUCC)
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions")
ELSEIF(MSVC71 OR MSVC80)
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /EHs-")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

IF(USES_SSE)
   IF(WIN32)
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DOGF_SSE /arch:SSE2 /Oi")
       SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /DOGF_SSE /arch:SSE2 /Oi")
   ELSE(WIN32)
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOGF_SSE -msse -msse2 -mfpmath=sse")
       SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DOGF_SSE -msse -msse2 -mfpmath=sse")
   ENDIF(WIN32)
ENDIF(USES_SSE)
 
IF(WIN32) 
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275 /wd4996")
       SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /wd4275 /wd4996")
ENDIF(WIN32)

ADD_SUBDIRECTORY(extern)


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
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern/OGF)
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
    basic
	math
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

