# @file CmakeList.txt
# @brief Macro configuration file.
# @author Alexander Pinzon Fernandez
# @date 21/02/2011 2:14 p.m.
IF(WIN32)
	SET(VISUAL_STUDIO_INCLUDE_HEADERS_IN_SOLUTION TRUE)
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