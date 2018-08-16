# GPTAM compilation for the web or Feedback about how to port an old non-trivial C++ software to WebAssembly
## Brief overview and launch instructions

This real-time Visual SLAM application is a rework of George Terzakis' GPTAM which is itself a deep code modification of the brilliant original work by Klein and Murray, *Parallel Tracking and Mapping* (PTAM).
It is an attempt to have GPTAM compiled to WebAssembly so it can be executed inside a web environment.
This project is born from the frustration of native applications not being “plug & play” enough for me.
The only thing you need to have installed on your computer, beside a recent browser, are Docker and Docker Compose.
After that, just `cd` into the *docker* directory and execute the command `docker-compose up`.
If you don't want to use Docker, you can execute directly on your system all the actions done by the [*Dockerfile*](https://github.com/Chadys/gptam/blob/master/docker/Dockerfile) and [*docker-compose.yml*](https://github.com/Chadys/gptam/blob/master/docker/docker-compose.yml) but I don't recommend it since that might mess up your system.
At the address <http://0.0.0.0:8080/> you'll find the html files produced by the application.
This project can currently work only on a Linux system because of the differences between OS to link hardware devices (camera) to a docker container.


## What is GPTAM and what is the use of this project

PTAM is a SLAM (Simultaneous Localization And Mapping) implementation. It can be used to locate a hand held camera in a restricted space in real time using only a video feed.
This program makes use of the methods explained by their authors in their papers *Parallel Tracking and Mapping for Small AR Workspace* and *Improving the Agility of Keyframe-Based SLAM*.
The first paper explains the use of two threads, one to generate a 3D map by identifying key-points, a second to continually locate the camera in that map while being robust to sudden moves. To accomplish these two objectives, an estimate of the most prominent planar surface in the environment will be calculated.
The second paper gives some methods for the localization to be even more resilient to sudden moves by using outline detection (used because outlines parallel to a move will not be blurred) and with a way to estimate the rotation angle by comparing less detailed and blurred versions of two frames.
I invite you to read both papers if you're interested in the specifics.
GPTAM is a rework with correction/modernization of the algorithms used in the original program and, more importantly, it removes all the dependencies of the original to rely only on OpenCV while also using it to simplify camera device access.

Concretely, GPTAM can be used to make applications that use augmented reality. Even while being ten years old, it is still one of the most performant open sourced AR tool.
By compiling to WebAssembly, you remove nearly all OS and hardware problematic and make your application works immediately on any device with a camera and a recent web browser, be it a computer or a phone.

## Progress made and what is left to be done

### Using Emscripten

The CMake file has been modified to use `emcc` and `em++` as compilers by the [toolchain file](https://github.com/kripken/emscripten/blob/master/cmake/Modules/Platform/Emscripten.cmake) provided by Emscripten (by using `emcmake` instead of `cmake`) to produce two html files, one for launching the camera calibrator and one for GPTAM itself, both compiled to Wasm.
The html files are then served by emrun to avoid using the FILE protocol because of its restrictive CORS rules.
For the commands to call and the options used to do that please look at the files [*launch.sh*](https://github.com/Chadys/gptam/blob/master/docker/launch.sh) and [*CMakeList.txt*](https://github.com/Chadys/gptam/blob/master/CMakeLists.txt).
Don't hesitate to look at the Emscripten documentation, it's very clear even if a bit lacking in example for non-trivial operations.

### Readline

The first problem I encountered was a missing library dependency : Readline. You can't just install it on your system and make it work because a code executed in a browser doesn't have access to the system executables.
The first rule to compile to WebAssembly is that all the program's dependencies must also be first compiled to WebAssembly. So I downloaded the [source code](http://ftp.gnu.org/gnu/readline/readline-6.3.tar.gz) of Readline and tried to compile it by simply calling `emcmake`, `emmake` and `emconfigure`.
But it failed, giving me a cryptic error about `SaHandler`.

Fortunately, to make NaCl works on Chromium and Chrome, Google first needed to compile lots of Linux libraries, [Readline included](https://chromium.googlesource.com/webports/+/master/ports/readline), inside an open source module called webports.
The key for a successful compilation of Readline seems to be in modifying some shell variables first. After doing so by using webports' [patch file for Readline](https://github.com/Chadys/gptam/blob/master/docker/readline-nacl.patch), I managed to obtain a file compiled in bitcode LLVM (which is the object code used by Emscripten), albeit with some warnings for undefined functions such as `tputs`.
Readline is very bound to the Linux architecture and uses some system libraries. The ecosystem is quite complex and the most simple way to correctly compile Readline would be to directly use [Google's webports tool](https://chromium.googlesource.com/webports.git) since it recently added experimental support for Emscripten.
Since these warnings didn't make the compilation fail, I decided to go back to it at a later time.

### X11

The second problem came from the choice of the technology used to create the OpenGL interface of PTAM. The authors didn't care back then about the compatibility with non UNIX system and used X11 windowing system.
In our case, any technology that's totally dependent on an architecture can't be ported to WebAssembly.
I decided to rewrite parts of GPTAM to use a compatible windowing system instead of X11. Some libraries have already been ported, and they can be used without difficulty in WebAssembly apps, like SDL or Freetype.
The `--show-ports` option of `emcc` command should normally list them all, but the list isn't up-to-date. The [official documentation](https://kripken.github.io/emscripten-site/docs/compiling/Building-Projects.html#emscripten-ports) lists some more ports, but I noticed that GLFW was also available by looking at a [pull request of the Emscripten repo](https://github.com/kripken/emscripten/pull/1887).
Since I already have experience with GLFW 3 and its usage is closer to the original X11 code than SDL, I ended up using it. I took advantage of this rewrite to also simplify the events' system (keyboard and mouse events).
The code that I changed might need some correction since, with an incomplete compilation, I couldn't test it yet.

### OpenGL

Emscripten can transcript code using OpenGL to code using WebGL. But that transcription has limits. Not all of OpenGL operations have a WebGL equivalent.
But most OpenGL ES 2 and 3 commands have a direct equivalent in WebGL 1 and 2, so limiting your code to use only compatible commands will simplify its portage.
Emscripten has options to emulate some non compatible features of OpenGL ES 2 and 3 and even of OpenGL 1.
An information I found out while testing and absent from [Emscripten documentation](https://kripken.github.io/emscripten-site/docs/porting/multimedia_and_graphics/OpenGL-support.html) : You can always activate OpenGL ES 3 emulation, but OpenGL ES 2 and OpenGL 1 emulation exclude each other.
The code of GPTAM uses OpenGL 2 with lots of features absent from WebGL. The version of OpenGL used is starting to get old (which was to be expected since PTAM original code has been written in 2008) and no GLSL shader have been used.
That make GPTAM strongly non compatible with WebGL. Activating OpenGL ES 3 and either OpenGL ES 2 or OpenGL 1 emulations gives the least number of compilation warnings.
But some functions like `glDrawPixels` do not have a web language equivalent by design and WebAssembly doesn't have a vocation to support old specification versions.
A whole rewrite of GPTAM interface in restricted OpenGL ES will be needed to make it works correctly in a browser.

### OpenCV

I was left with the OpenCV problem. Like I said earlier, all of a Wasm project's dependencies must first also be compiled to Wasm.
OpenCV is not, yet, one of the Emscripten ports and I didn't feel capable of porting it all myself.
Fortunately, I wasn't the first one interested by a web compatible version of OpenCV. A few years ago, Sajjad Taheri, Alexander Veidenbaum, Alexandru Nicolau and Mohammad R. Haghighat wrote OpenCV.js.
This project gives access to a subpart of OpenCV and it has by then been fully integrated to OpenCV with an [installation documentation](https://docs.opencv.org/3.4.0/d4/da1/tutorial_js_setup.html) available.
Since OpenCV.js was to made for OpenCV 3, the first thing to do was make GPTAM depends on OpenCV 3.4.1 instead of version 2.4.13 like originally. Only a header change in the code was necessary for that.

OpenCV.js compilation is done via a Python script [*build_js.py*](https://github.com/opencv/opencv/blob/master/platforms/js/build_js.py) which is going to launch CMake with Wasm necessary compilation options.
The problem is that a lot of OpenCV modules haven't been ported to Wasm yet so their activation is not done in this file. I modified it so that GPTAM's necessary extensions are active : 'calib3d', 'features2d', 'photo', 'imgcodecs', 'videoio' et 'highgui'.
After that I got compilation errors telling me that the modules I just added were absent from the build folder. I needed to take a closer look to the compilation process.

The compilation of a library to WebAssembly is done using [bindings](https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/embind.html), because Emscripten tools takes upon themselves to delete unused code since file size is very important in a web environment (the less information to be transmitted from server to client, the better).
The bindings directives tell Emscripten not to delete code from specific function and since no function is ever directly called in a library contrarily to in an executable, bindings are mandatory.
OpenCV.js compilation is done using scripts to automatically generate bindings since there are to many to write them by hand. Those scripts dynamically generate a C++ file describing all the bindings and add it to the source files to be compiled. This dynamic file is based on a file already containing lots of bindings, [*core_bindings.cpp*](https://github.com/opencv/opencv/blob/master/modules/js/src/core_bindings.cpp) that are useful no matter the module ported.
For that file generation, the global [*CMakeList.txt*](https://github.com/opencv/opencv/blob/92fb3fb33fdafec87867a6531aacf40a190caec2/modules/js/CMakeLists.txt) of OpenCV will go through each module and, if the *CMakeList.txt* of that module indicate that it should be wrapped to js, add it to a list passed as argument to [*embindgen.py*](https://github.com/opencv/opencv/blob/master/modules/js/src/embindgen.py).
This last script use the parser that was already written to port OpenCV to Python to get the declaration of each function, class, constant, enumeration, *etc* in a module.
For each declaration, the script first checks that its name is contained inside of its `white_list` and then uses a [templating script](https://github.com/opencv/opencv/blob/89b6e68e1eea83262ef79c2110df111dbc089b97/modules/js/src/templates.py) that will put the declaration in the correct format to add it to the dynamically created bindings file.

This process tell us that several modifications are needed if we want to compile OpenCV.js with more modules.
I wrote a [Python script](https://github.com/Chadys/gptam/blob/master/docker/enable_components_opencvjs.py) to automate all the necessary modifications. It will add `'js'` to the `wraps` of the module to indicate the possibility to port it to a web compatible language, will add necessary includes to the base bindings file *core_bindings.cpp* and will add wanted elements to the `white_list` of *embindgen.py*.
After all that, I finally had a Wasm compilation of GPTAM that was, if not functional, at least that did complete to the end.

Some remarks : the WebAssembly ecosystem is growing fast and all the work that I did here might soon not be necessary. The last module to have been added to OpenCV.js, 'dnn', has been included the Eleventh of December 2017 and I sure hope it won't be the last.
While exploring the compilation strategy of OpenCV.js, I also noticed a constraint : the goal of OpenCV.js is to make available OpenCV from JavaScript code. But JavaScript isn't a typed language so, if a function or method overload another one with the same number of arguments, even if the arguments are of different type, only one overload will be made available to the JavaScript code.

### Off topic : Adding a Wasm compatible Windowing interface for 'highgui'

Even through none is used by GPTAM, the 'highgui' module give access to display functions such as `imshow`. For that, it needs a windowing system.
The supported ones are Win32ui and WinRT, which run on Windows, Cocoa and Carbon, which run on OSX, GTK which runs on Linux and finally QT which is the only cross platform one.
Like always, to compile to Wasm, all the dependencies must be compiled to Wasm first. An architecture specific windowing system is not an option, moreover some of them aren't even open sourced, so QT is the only option left.
But QT hasn't been ported to Wasm yet, hence why I looked into how to had support of other windowing systems to OpenCV.

The process is not that complex : first you need to edit the [file defining all the constants](https://github.com/opencv/opencv/blob/master/cmake/templates/cvconfig.h.in) used by CMake for the installation to add a constant that will be used to indicate the presence of the new library.
Then you edit the [file that detect the presence of a supported graphic library and that add it to the dependencies of OpenCV](https://github.com/opencv/opencv/blob/master/cmake/OpenCVFindLibsGUI.cmake) so that it detects you new windowing system.
You need to add to the [graphic functions of OpenCV](https://github.com/opencv/opencv/blob/master/modules/highgui/src/window.cpp) the call to new functions making use of your chosen windowing system and you need to write the file that will contain the definition of those functions (example available for [Carbon](https://github.com/opencv/opencv/blob/master/modules/highgui/src/window_carbon.cpp)).
After that you edit the [*CMakeLists.txt* of the 'highgui' module](https://github.com/opencv/opencv/blob/master/modules/highgui/CMakeLists.txt) so that it adds your new file to the module's sources.
Finally, you need to add [tests for the new windowing system](https://github.com/opencv/opencv/blob/master/modules/highgui/test/test_gui.cpp).

The only time-consuming step is to write the file containing the bindings between OpenCV's functions and the library we're adding.
I wrote a [Python script to automate all of those steps](https://github.com/Chadys/gptam/blob/master/docker/enable_glfw_windowing_system.py) (I wrote it with GLFW in mind, hence the name for the new file and constant) and nearly finished the modification to the [*window.cpp* file](https://github.com/Chadys/gptam/blob/master/docker/window.cpp). All that is left to do is to complete the [bindings file](https://github.com/Chadys/gptam/blob/master/docker/window_glfw.cpp).
Since GPTAM doesn't use OpenCV's graphic display functions, I didn't finish it ; moreover QT port to Wasm is a work-in-progress so adding another windowing system might soon not be needed to have those functions accessible in a browser.
But I found it interesting on itself to look into how to add new windowing systems to OpenCV and, beside, making available another cross platform option, and a lighter one on top of that, might be appreciated by the community of OpenCV users.

### File system

A Wasm program of course doesn't have access to the computer's file system. But Emscripten provide a [virtual file system](https://kripken.github.io/emscripten-site/docs/api_reference/Filesystem-API.html).
Since GPTAM reads and writes files to manage the settings of the camera, we needed to activate that functionality. It activates automatically as soon as you add the option to preload a file, so I only needed to ask for the preloading of the calibrator configuration file to enable all future file operations.

### Async main loop

Graphic applications make use of an infinite loop to update the display and manage hardware events.
It causes problems in a web environment because the browser will not be able to regain control of the code execution and will detect the page as if it was frozen.
Emscripten provide a [function](https://kripken.github.io/emscripten-site/docs/porting/emscripten-runtime-environment.html#browser-main-loop), `emscripten_set_main_loop`, which indicates what function needs to be executed continually and at what frequency (useful to have control over fps).
A call to `emscripten_cancel_main_loop` stops the loop, which remove the need of a boolean variable tested in the while condition.
This constraint required a slight code refactoring. The only difficulty came from the need to operate the loop function with a class method instead of a static one.
For that, I used the `emscripten_set_main_loop_arg` variant.

### Threads

Thanks to the *SharedArrayBuffer* API, it becomes possible to share memory between the main thread and web workers (the parallelization technology of the web).
Emscripten uses it to [compile C code that uses POSIX threads](https://kripken.github.io/emscripten-site/docs/porting/pthreads.html).
But its support is still experimental, moreover this API is disabled by default on browsers because there are still some security problems with it.
In GPTAM, threads support is mandatory, so I activated the option to enable threads support but for now I still didn't manage to compile without error with this option.

### Access to camera

GPTAM use the `VideoCapture` tool of OpenCV to access the user's camera feed. But this class implementation can't work in a web browser.
The authors of OpenCV.js voluntarily didn't look onto this functionality because the goal they had in mind for OpenCV.js was to be used as a JavaScript module and not to enable to compile other software to Wasm.
And camera capture is trivially done in JavaScript by using `navigator.mediaDevices.getUserMedia`, so non need for the OpenCV function there.

So how to get access to the camera in Wasm ? There is a way to call JavaScript function and to get their result from C++ code, but it only works easily with simple objects : `int`, `double` and, with some difficulty, `string` ; that is because value types in Wasm can only be one of i32, i64, f32 or f64.
Furthermore, `getUserMedia` does not directly return the video object, but a `Promise` object which will give access to the camera feed in an asynchronous event only if the user gives its consent for the access to their camera.
For the time being, I didn't find how to bypass this problem and retrieve the camera feed directly in the Wasm code.
An interesting lead could be the use of another [Emscripten's function](https://kripken.github.io/emscripten-site/docs/api_reference/Filesystem-API.html#devices) `FS.registerDevice`.

## Final remarks

In this exploratory project, I found both the potential and the limitation of WebAssembly with the current tools at our disposal.
In simple projects, all you need to do to compile your code to Wasm is to add the needed options to the compiler, to launch compilation with the tools provided by Emscripten, and, eventually, to modify the way you link your dependencies if you use some Wasm ports.
Complexity arises when you are in need of unsupported dependencies, and hardware functionalities (threads, device) are still a problem.
Once you are aware of Emscripten limits, producing a code that will pose zero compilation difficulty is easy. However, when you try to compile a code that was written with a single architecture in mind, like GPTAM, you will surely meet challenges along the way.
A lot of work still needs to be done for the completion of this GPTAM port to Wasm but, little by little, as technology evolve, most of the challenges I met will probably become trivial to overcome.

Feel free to help me finish this project :) !