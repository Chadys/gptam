newContent = []
with open('/opt/opencv/opencv-3.4.1/modules/js/src/embindgen.py', 'r') as f:
    while True:
        line = f.readline()
        if not line:
            break
        if line.startswith('white_list = '):
            # add functions of modules used by gptam to exported list
            newContent.extend(["videoio = {}\n",
                          "features2d = {'' : []}\n",
                          "imgcodecs = {}\n",
                          "calib3d = {'' : ['fisheye.initUndistortRectifyMap']}\n",
                          "photo = {}\n",
                          "highgui = {}\n",
            # add modules not normally exported to js
                          'white_list = makeWhiteList([core, imgproc, objdetect, video, dnn, videoio, features2d, imgcodecs, calib3d, photo, highgui])\n'])
            continue
        # enable enums export
        if line.startswith('export_enums'):
            newContent.append('export_enums = True\n')
            break
        newContent.append(line)
    newContent.append(f.read())
with open('/opt/opencv/opencv-3.4.1/modules/js/src/embindgen.py', 'w') as f:
    f.writelines(newContent)

newContent = []
with open('/opt/opencv/opencv-3.4.1/modules/js/src/core_bindings.cpp', 'r') as f:
    while True:
        line = f.readline()
        if not line:
            break
        if line == '#include <emscripten/bind.h>\n':
            # import modules to corebindings
            newContent.extend(['#include "opencv2/videoio.hpp"\n',
                               '#include "opencv2/videoio/container_avi.private.hpp"\n',
                               '#include "opencv2/features2d.hpp"\n',
                               '#include "opencv2/imgcodecs.hpp"\n',
                               '#include "opencv2/calib3d.hpp"\n',
                               '#include "opencv2/photo.hpp"\n',
                               '#include "opencv2/highgui.hpp"\n',
                               line])
            break
        newContent.append(line)
    newContent.append(f.read())
with open('/opt/opencv/opencv-3.4.1/modules/js/src/core_bindings.cpp', 'w') as f:
    f.writelines(newContent)

for module in ['videoio', 'features2d', 'imgcodecs', 'calib3d', 'photo', 'highgui']:
    newContent = []
    fileName = '/opt/opencv/opencv-3.4.1/modules/'+module+'/CMakeLists.txt'
    with open(fileName, 'r') as f:
        for line in f:
            if 'ocv_define_module' in line or 'ocv_add_module' in line:
                # add js to module's wrap
                newContent.append(line[:-2] + ' js)\n')
                continue
            newContent.append(line)
    with open(fileName, 'w') as f:
        f.writelines(newContent)