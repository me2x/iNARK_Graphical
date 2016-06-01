cmake_minimum_required (VERSION 3.0.2)
project(iNARK_Graphical)

set(popup_SRCS
l1_popup.cpp
l2_popup.cpp
l3_popup.cpp
l4_popup.cpp
l5_popup.cpp
ports_popup.cpp
)
message(${binary_dir})
add_library (popups ${popup_SRCS})
add_dependencies(popups demo) #force the dependency solution
target_link_libraries( popups ${binary_dir}/libinark_commons.a)#linking is performed here. the name is the one in the CMakeLIst of the project.
