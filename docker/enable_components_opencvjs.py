with open('/opt/opencv/opencv-3.4.1/modules/js/src/embindgen.py', 'r+') as f:
    while True:
        line = f.readline()
        if not line:
            break
        if line.startswith('white_list = '):
            # add functions of modules used by gptam to exported list
            f.writelines(['videoio = {}',
                          'features2d = {}',
                          'imgcodecs = {}',
                          'calib3d = {}',
                          'photo = {}',
                          'highgui = {}'])
            # add modules not normally exported to js
            f.write('white_list = makeWhiteList([core, imgproc, objdetect, video, dnn, videoio, features2d, imgcodecs, calib3d, photo, highgui])')
            break
        f.write(line)
    f.writelines(f.read())

with open('/opt/opencv/opencv-3.4.1/modules/js/src/core_bindings.cpp', 'r+') as f:
    while True:
        line = f.readline()
        if not line:
            break
        if line == '#include <emscripten/bind.h>':
            # import modules to corebindings
            f.writelines(['#include "opencv2/videoio.hpp',
                          '#include "opencv2/features2d.hpp',
                          '#include "opencv2/imgcodecs.hpp',
                          '#include "opencv2/calib3d.hpp',
                          '#include "opencv2/photo.hpp',
                          '#include "opencv2/highgui.hpp',
                          line])
            break
        f.write(line)
    f.writelines(f.read())

for fileName in ['videoio', 'features2d', 'imgcodecs', 'calib3d', 'photo', 'highgui']:
    with open('/opt/opencv/opencv-3.4.1/modules/'+fileName+'/CMakeLists.txt', 'r+') as f:
        for line in f:
            if 'ocv_define_module' in line or 'ocv_add_module' in line:
                # add js to module's wrap
                f.write(line[:-1] + ' js)')
                continue
            f.write(line)