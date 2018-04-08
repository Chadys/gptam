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
COPY ./build_js.py /opt/opencv/opencv-3.4.1/platforms/js/build_js.py
RUN ["/bin/bash", "-c", "source /opt/emsdk/emsdk_env.sh\
        && python /opt/opencv/opencv-3.4.1/platforms/js/build_js.py /opt/opencv/build_js --build_wasm"]
WORKDIR /opt/code/
EXPOSE 8080