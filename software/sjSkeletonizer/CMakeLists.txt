###### BEGIN GPL LICENSE BLOCK#####
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# The Original Code is Copyright (C) 2007-2010 by Alexander Pinzon Fernandez.
# Bogota - Colombia
# All rights reserved.
#
# Author(s): Alexander Pinzón Fernández.
#
###### END GPL LICENSE BLOCK#####
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

SET(CMAKE_MODULE_PATH ${sjSkeletonizer_SOURCE_DIR}/CMakeModules)
INCLUDE(sjMacro)
SET(BUILD_SHARED_LIBS ON)

SET(BINARIES_RELATIVE_PATH /bin/)
SET(LIBRARIES_RELATIVE_PATH /bin/)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}${LIBRARIES_RELATIVE_PATH})
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

IF (MSVC)
	# Use /FR to turn on IntelliSense
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FR")
ENDIF()

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

ADD_SUBDIRECTORY(extern)
ADD_SUBDIRECTORY(source)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(laplacian_plugin)
INCLUDE(sjInstallFiles)