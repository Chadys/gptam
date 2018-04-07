#!/bin/bash
source /opt/emsdk/emsdk_env.sh &&
emcmake cmake &&
emmake make -j4 &&
emrun --no_browser --hostname 0.0.0.0 --port 8080 .