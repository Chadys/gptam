# add HAVE_GLFW constant
with open('/opt/opencv/opencv-3.4.1/cmake/OpenCVFindLibsGUI.cmake', 'a') as f:
    f.writelines([
        'if(WITH_GLFW)\n',
        '   set(HAVE_GLFW YES)\n',
        #TODO '    set(HAVE_OPENGL TRUE)\n',
        'endif()\n'
    ])

newContent = []
with open('/opt/opencv/opencv-3.4.1/modules/highgui/test/test_gui.cpp', 'r') as f:
    while True:
        line = f.readline()
        if not line:
            break
        if line.startswith('#if defined HAVE_GTK'):
            # add HAVE_GLFW as meaning windowing system is working and can be tested
            newContent.append(line[:-1]+' || defined HAVE_GLFW\n')
            break
        newContent.append(line)
    newContent.append(f.read())
with open('/opt/opencv/opencv-3.4.1/modules/js/src/embindgen.py', 'w') as f:
    f.writelines(newContent)

newContent = []
with open('/opt/opencv/opencv-3.4.1/modules/highgui/CMakeLists.txt', 'r') as f:
    while True:
        line = f.readline()
        if not line:
            break
        if line == 'elseif(HAVE_COCOA)\n':
            # include window_glfw as source for highgui
            newContent.extends([line, f.readline(), f.readline(),
                                'elseif(HAVE_GLFW)\n',
                                ' list(APPEND highgui_srcs ${CMAKE_CURRENT_LIST_DIR}/src/window_glfw.cpp)'
                                ])
            break
        newContent.append(line)
    newContent.append(f.read())
with open('/opt/opencv/opencv-3.4.1/modules/js/src/embindgen.py', 'w') as f:
    f.writelines(newContent)


newContent = []
with open('/opt/opencv/opencv-3.4.1/cmake/templates/cvconfig.h.in', 'r') as f:
    for line in f:
        newContent.append(line)
newContent.insert(-1, '#cmakedefine HAVE_GLFW\n')
with open('/opt/opencv/opencv-3.4.1/modules/js/src/embindgen.py', 'w') as f:
    f.writelines(newContent)