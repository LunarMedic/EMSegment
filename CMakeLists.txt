# --------------------------------------------------------------------------
#
# EM Segmenter for 3D Slicer3 and 3D Slicer4
#
# --------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
if(POLICY CMP0016)
  CMAKE_POLICY(SET CMP0016 NEW)
endif()
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW) # CMake 3.0.0
endif()
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW) # CMake 2.8.11
endif()

#-----------------------------------------------------------------------------
set(EXTENSION_NAME EMSegment)

# --------------------------------------------------------------------------
if(Slicer3_SOURCE_DIR)
  include(CMake/EM_Slicer3_FindPackageSlicer.cmake)
else()
  include(CMake/EM_Slicer4_FindPackageSlicer.cmake)
endif()

# --------------------------------------------------------------------------
include(CMake/EM_BaseSources.cmake)

# --------------------------------------------------------------------------
if("${Slicer_VERSION_MAJOR}" EQUAL "3")
  include(CMake/EM_Slicer3_IncludeSlicerPython.cmake)
  include(CMake/EM_Slicer3_GenerateConfigureFile.cmake)
endif()

# --------------------------------------------------------------------------
ADD_SUBDIRECTORY(CommandLineApplication)

# --------------------------------------------------------------------------
add_subdirectory(Qt)

# --------------------------------------------------------------------------
if(BUILD_TESTING)
  ADD_SUBDIRECTORY(Testing)
endif()

# --------------------------------------------------------------------------
include(CMake/EM_InstallTasks.cmake)

# --------------------------------------------------------------------------
add_custom_target(copytesting ALL
  ${CMAKE_COMMAND}
  -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestData
  -DDST=${CMAKE_BINARY_DIR}/${EM_SHARE_DIR}/${EXTENSION_NAME}/Testing
  -P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/EM_InstallTasksSubScript.cmake
  )

# --------------------------------------------------------------------------
if("${Slicer_VERSION_MAJOR}" EQUAL "4")
  include(CMake/EM_Slicer4_GenerateExtensionFile.cmake)
endif()
