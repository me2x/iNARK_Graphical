cmake_minimum_required(VERSION 3.0.2)
project(iNARK_Graphical)
find_package(Qt4 4.4.3 REQUIRED)

include(ExternalProject)

set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_SOURCE_DIR}/src/commons) #folder where the project will be imported. 

ExternalProject_Add(demo
  GIT_REPOSITORY https://github.com/me2x/iNARK_Commons.git
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(demo source_dir) #gets automatically the main directory of the imported project
include_directories(${source_dir})
message(${source_dir})
ExternalProject_Get_Property(demo binary_dir) #automatically gets the build directory
message(${binary_dir})
add_subdirectory(popups)

set(iNARK_Graphical_SRCS
maininterface.cpp
mainwindow.cpp
main.cpp
graphic_edge.cpp
graphic_vertex.cpp
logical_vertex.cpp
logical_edge.cpp
handleitem.cpp
)



add_executable(iNARK_Graphical ${iNARK_Graphical_SRCS})
add_dependencies(iNARK_Graphical demo) #force the dependency solution
add_dependencies(iNARK_Graphical popups)

target_link_libraries(iNARK_Graphical popups ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${binary_dir}/libinark_commons.a) #linking is performed here. the name is the one in the CMakeLIst of the project.

install(TARGETS iNARK_Graphical RUNTIME DESTINATION bin)
