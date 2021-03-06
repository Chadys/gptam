FROM ubuntu:16.04
RUN apt-get update
RUN apt-get install -y git python-dev build-essential
RUN mkdir -p /opt/code/
RUN cd /opt/ && git clone https://github.com/juj/emsdk.git
RUN apt-get install -y cmake
RUN cd /opt/emsdk && \
./emsdk install latest && \
./emsdk activate latest
RUN apt-get install -y wget unzip
RUN mkdir /opt/opencv && cd /opt/opencv\
    && wget 'https://github.com/opencv/opencv/archive/3.4.1.zip' -O opencv-3.4.1.zip \
    && unzip opencv-3.4.1.zip \
    && rm opencv-3.4.1.zip
RUN mkdir /opt/readline && cd /opt/readline\
    && wget 'http://ftp.gnu.org/gnu/readline/readline-6.3.tar.gz' -O readline-6.3.tar.gz \
    && tar -xzf readline-6.3.tar.gz \
    && rm readline-6.3.tar.gz
COPY ./readline-nacl.patch /opt/readline/readline-6.3/readline-nacl.patch
RUN ["/bin/bash", "-c", "source /opt/emsdk/emsdk_env.sh\
        && cd /opt/readline/readline-6.3\
        && git apply readline-nacl.patch\
        && export bash_cv_wcwidth_broken=no\
        && export bash_cv_signal_vintage=posix\
        && export ac_cv_lib_util_openpty=no\
        && export ac_cv_lib_dir_opendir=no\
        && $EMSCRIPTEN/emconfigure ./configure --disable-shared\
        && $EMSCRIPTEN/emmake make\
        && $EMSCRIPTEN/emmake make install"]
COPY ./build_js.py /opt/opencv/opencv-3.4.1/platforms/js/build_js.py
COPY ./enable_components_opencvjs.py /opt/opencv/opencv-3.4.1/enable_components_opencvjs.py
RUN ["python", "/opt/opencv/opencv-3.4.1/enable_components_opencvjs.py"]
#COPY ./enable_glfw_windowing_system.py /opt/opencv/opencv-3.4.1/enable_glfw_windowing_system.py
#RUN ["python", "/opt/opencv/opencv-3.4.1/enable_glfw_windowing_system.py"]
#COPY ./window.cpp /opt/opencv/opencv-3.4.1/modules/highgui/src/window.cpp
#COPY ./window_glfw.cpp /opt/opencv/opencv-3.4.1/modules/highgui/src/window_glfw.cpp
RUN ["/bin/bash", "-c", "source /opt/emsdk/emsdk_env.sh\
        && python /opt/opencv/opencv-3.4.1/platforms/js/build_js.py /opt/opencv/build_js --build_wasm --clean_build_dir\
        || (cat /opt/opencv/build_js/CMakeFiles/CMakeOutput.log && cat /opt/opencv/build_js/CMakeFiles/CMakeError.log && 0)"]
WORKDIR /opt/code/
EXPOSE 8080
