#
# Install (copy) the task related .tcl and .mrml files to the appropriate places
#

ADD_CUSTOM_TARGET(copytasks ALL
  ${CMAKE_COMMAND}
  -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/Tasks
  -DDST=${CMAKE_BINARY_DIR}/${EM_SHARE_DIR}/${EXTENSION_NAME}/
  -P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/EM_InstallTasksSubScript.cmake
)

install(
  DIRECTORY Tasks
  DESTINATION ${EM_INSTALL_SHARE_DIR}/${EXTENSION_NAME}/
  FILES_MATCHING PATTERN "*"
  REGEX "/\\.svn" EXCLUDE
)

ADD_CUSTOM_TARGET(copytcl ALL
  ${CMAKE_COMMAND}
  -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/Tcl
  -DDST=${CMAKE_BINARY_DIR}/${EM_SHARE_DIR}/${EXTENSION_NAME}/
  -P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/EM_InstallTasksSubScript.cmake
)

install(
  DIRECTORY Tcl
  DESTINATION ${EM_INSTALL_SHARE_DIR}/${EXTENSION_NAME}/
  FILES_MATCHING PATTERN "*"
  REGEX "/\\.svn" EXCLUDE
)


