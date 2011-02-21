IF(CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)

	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern/OGF)
	INCLUDE(AddFileDependencies)
	INCLUDE_DIRECTORIES(${QGLVIEWER_INCLUDE_DIR})
	INCLUDE_DIRECTORIES(${TAUCS_INCLUDE_DIR})

	qt4_generate_moc( "${CMAKE_SOURCE_DIR}/include/sjViewer.h" ${CMAKE_CURRENT_BINARY_DIR}/sjViewer_moc.cpp )
	SET(FOO_MOC_FILE "${CMAKE_CURRENT_BINARY_DIR}/sjViewer_moc.cpp")
	add_file_dependencies( ${FOO_MOC_FILE} "${CMAKE_SOURCE_DIR}/include/sjViewer.h" )
	
	#SET(QtApp_RCC_SRCS sjViewer.qrc)

	#QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCC_SRCS})

	
	SET(sjSkeletonizer_SOURCE
		${FOO_MOC_FILE}
		${CMAKE_SOURCE_DIR}/include/sjConfig.h
		${CMAKE_SOURCE_DIR}/include/sjDataIO.h
		${CMAKE_SOURCE_DIR}/include/sjDataType.h
		${CMAKE_SOURCE_DIR}/include/sjLaplacianSmoothing.h
		${CMAKE_SOURCE_DIR}/include/sjUtils.h
		${CMAKE_SOURCE_DIR}/include/sjViewer.h
		sjDataIO.cpp
		sjLaplacianSmoothing.cpp
		sjMain.cpp
		sjUtils.cpp
		sjViewer.cpp
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
		${TAUCS_LIBRARIES_DIR}/libtaucs
	)

	add_to_cached_list( CGAL_EXECUTABLE_TARGETS qtviewer )
	INSTALL_TARGETS(/bin sjSkeletonizer)

ELSE (CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)
    message(STATUS "THE REQUIRED LIBRARIES ARE NOT PRESENT.")
ENDIF (CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)