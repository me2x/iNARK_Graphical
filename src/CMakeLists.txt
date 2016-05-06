cmake_minimum_required(VERSION 3.0.2)
project(iNARK_Graphical)
find_package(Qt4 4.4.3 REQUIRED)


set(iNARK_Graphical_SRCS
maininterface.cpp
mainwindow.cpp
main.cpp
graphic_edge.cpp
graphic_vertex.cpp
)


add_executable(iNARK_Graphical ${iNARK_Graphical_SRCS})
target_link_libraries(iNARK_Graphical ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS iNARK_Graphical RUNTIME DESTINATION bin)
# i common sono definiti nella graph library. quindi va aggiunta. inoltre se voglio usare la rappresentazione logica serve avere le classi source graph e internal graph qua quindi va inclusa tutta la logic library
#target_link_libraries(graphical ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} graph)