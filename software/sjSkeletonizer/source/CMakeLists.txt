IF(CGAL_Qt4_FOUND AND QT4_FOUND AND OPENGL_FOUND AND QGLVIEWER_FOUND)

	ADD_DEFINITIONS(-DSJENGINE_EXPORTS)
	IF(WIN32)
	  ADD_DEFINITIONS(-DWIN32)
	  ADD_DEFINITIONS(-D_WINDOWS)
	  ADD_DEFINITIONS(-D_MBCS)
	ENDIF(WIN32)

	INCLUDE_DIRECTORIES (exception)
	INCLUDE_DIRECTORIES (io)
	INCLUDE_DIRECTORIES (log)
	INCLUDE_DIRECTORIES (parameters)
	INCLUDE_DIRECTORIES (plugin)
	INCLUDE_DIRECTORIES (system)
	INCLUDE_DIRECTORIES (utils)
	INCLUDE_DIRECTORIES (xml)
	
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern/OGF)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern/log4cplus/include)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/extern/libxml2/include)
	INCLUDE(AddFileDependencies)
	INCLUDE_DIRECTORIES(${QGLVIEWER_INCLUDE_DIR})
	INCLUDE_DIRECTORIES(${TAUCS_INCLUDE_DIR})

	AUX_SOURCE_DIRECTORY(exception exception_sources)
	AUX_SOURCE_DIRECTORY(io io_sources)
	AUX_SOURCE_DIRECTORY(log log_sources)
	AUX_SOURCE_DIRECTORY(parameters parameters_sources)
	AUX_SOURCE_DIRECTORY(plugin plugin_sources)
	AUX_SOURCE_DIRECTORY(system system_sources)	
	AUX_SOURCE_DIRECTORY(utils utils_sources)
	AUX_SOURCE_DIRECTORY(xml xml_sources)
	
	IF(WIN32)
		GatherProjectFiles(exception source/exception exception_headerFiles)
		GatherProjectFiles(io source/io io_headerFiles)
		GatherProjectFiles(log source/log log_headerFiles)
		GatherProjectFiles(parameters source/parameters parameters_headerFiles)
		GatherProjectFiles(plugin source/plugin plugin_headerFiles)
		GatherProjectFiles(system source/system system_headerFiles)
		GatherProjectFiles(utils source/utils utils_headerFiles)
		GatherProjectFiles(xml source/xml xml_headerFiles)
		
		SET(exception_sources 		${exception_sources} 	${exception_headerFiles})
		SET(io_sources 				${io_sources} 			${io_headerFiles})
		SET(log_sources 			${log_sources} 			${log_headerFiles})
		SET(parameters_sources 		${parameters_sources} 	${parameters_headerFiles})
		SET(plugin_sources 			${plugin_sources} 		${plugin_headerFiles} )
		SET(system_sources 			${system_sources} 		${system_headerFiles} )
		SET(utils_sources 			${utils_sources} 		${utils_headerFiles} )
		SET(xml_sources 			${xml_sources} 			${xml_headerFiles})
	ENDIF(WIN32)

	ADD_LIBRARY(log STATIC ${log_sources} )
	TARGET_LINK_LIBRARIES (log log4cplus )
	
	ADD_LIBRARY(exception STATIC ${exception_sources} )
	TARGET_LINK_LIBRARIES (exception log log4cplus)
	ADD_LIBRARY(io STATIC ${io_sources} )
	TARGET_LINK_LIBRARIES (io 
		${CGAL_LIBRARIES} 
		${CGAL_3RD_PARTY_LIBRARIES} 
		${TAUCS_LIBRARIES_DIR}/libtaucs
		log4cplus )	
	
	ADD_LIBRARY(parameters STATIC ${parameters_sources} )
	TARGET_LINK_LIBRARIES (parameters exception log log4cplus)

	ADD_LIBRARY(system STATIC ${system_sources})
	TARGET_LINK_LIBRARIES (utils 
		io
		libxml2
		parameters
		plugin
		utils
		xml
		${CGAL_LIBRARIES} 
		${CGAL_3RD_PARTY_LIBRARIES} 
		${TAUCS_LIBRARIES_DIR}/libtaucs)
	
	ADD_LIBRARY(utils STATIC ${utils_sources})
	TARGET_LINK_LIBRARIES (utils 
		io
		${CGAL_LIBRARIES} 
		${CGAL_3RD_PARTY_LIBRARIES} 
		${TAUCS_LIBRARIES_DIR}/libtaucs)
	
	ADD_LIBRARY(plugin STATIC ${plugin_sources})

	ADD_LIBRARY(xml STATIC ${xml_sources})
	TARGET_LINK_LIBRARIES (xml exception libxml2)

	qt4_generate_moc( "${CMAKE_SOURCE_DIR}/source/gui/sjViewer.h" ${CMAKE_CURRENT_BINARY_DIR}/sjViewer_moc.cpp )
	SET(FOO_MOC_FILE "${CMAKE_CURRENT_BINARY_DIR}/sjViewer_moc.cpp")
	add_file_dependencies( ${FOO_MOC_FILE} "${CMAKE_SOURCE_DIR}/gui/sjViewer.h" )
	
	qt4_generate_moc( "${CMAKE_SOURCE_DIR}/source/gui/sjApplication.h" ${CMAKE_CURRENT_BINARY_DIR}/sjApplication_moc.cpp )
	SET(FOO_MOC_FILE2 "${CMAKE_CURRENT_BINARY_DIR}/sjApplication_moc.cpp")
	add_file_dependencies( ${FOO_MOC_FILE2} "${CMAKE_SOURCE_DIR}/gui/sjApplication.h" )
	
	qt4_generate_moc( "${CMAKE_SOURCE_DIR}/source/gui/sjPluginWidget.h" ${CMAKE_CURRENT_BINARY_DIR}/sjPluginWidget_moc.cpp )
	SET(FOO_MOC_FILE3 "${CMAKE_CURRENT_BINARY_DIR}/sjPluginWidget_moc.cpp")
	add_file_dependencies( ${FOO_MOC_FILE3} "${CMAKE_SOURCE_DIR}/gui/sjPluginWidget.h" )
	
	#SET(QtApp_RCC_SRCS sjViewer.qrc)

	#QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCC_SRCS})
	
	SET(sjSkeletonizer_SOURCE
		${FOO_MOC_FILE}
		${FOO_MOC_FILE2}
		${FOO_MOC_FILE3}
		gui/sjApplication.h
		gui/sjViewer.h
		gui/sjPluginWidget.h
		gui/sjApplication.cpp
		gui/sjMain.cpp
		gui/sjViewer.cpp
		gui/sjPluginWidget.cpp
	)
	

	#ADD_EXECUTABLE ( sjSkeletonizer ${sjSkeletonizer_SOURCE} ${UI_FILES} ${QtApp_RCC_SRCS})
	ADD_EXECUTABLE ( sjSkeletonizer ${sjSkeletonizer_SOURCE} )

	TARGET_LINK_LIBRARIES ( sjSkeletonizer 
		exception
		io
		log
		parameters
		plugin
		utils
		xml
		basic
		system
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