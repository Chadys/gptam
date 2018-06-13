cmake_minimum_required(VERSION 3.4)

set(GPTAM_PROJ_NAME gptam)
set(CALIB_PROJ_NAME gcalibrator)

set(CMAKE_CXX_STANDARD 14)



######################################
# The external reference directory (searching) for cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


## find openCV
set(OpenCV_STATIC ON)
set(OpenCV_DIR /opt/opencv/build_js)
find_package(OpenCV 3 REQUIRED)

list( APPEND
	EXT_INCLUDE_DIRS
	${OpenCV_INCLUDE_DIRS}
)

list( APPEND
	EXT_LIBS
	${OpenCV_LIBS}
)

## find OpenGL
find_package(OPENGL REQUIRED)
list( APPEND
	EXT_INCLUDE_DIRS
	${OPENGL_INCLUDE_DIR}
)
list( APPEND
	EXT_LIBS
	${OPENGL_LIBRARIES}
)

## find Readline
list( APPEND
        EXT_INCLUDE_DIRS
        /usr/local/include
        )
list( APPEND
        EXT_LIBS
        /usr/local/lib/libhistory.a  /usr/local/lib/libreadline.a
		)

########## 1. GPTAM SOURCE FILES  ###################

set(GPTAM_PROJ_SOURCE
	${CMAKE_SOURCE_DIR}/main.cpp
	${CMAKE_SOURCE_DIR}/ARDriver.cpp
	${CMAKE_SOURCE_DIR}/System.cpp
	${CMAKE_SOURCE_DIR}/KeyFrame.cpp
	${CMAKE_SOURCE_DIR}/MapPoint.cpp
	${CMAKE_SOURCE_DIR}/Map.cpp
	${CMAKE_SOURCE_DIR}/MapViewer.cpp
	${CMAKE_SOURCE_DIR}/PatchFinder.cpp
	${CMAKE_SOURCE_DIR}/MapMaker.cpp
	${CMAKE_SOURCE_DIR}/Tracker.cpp
	${CMAKE_SOURCE_DIR}/Relocaliser.cpp
	${CMAKE_SOURCE_DIR}/HomographyInit.cpp
	${CMAKE_SOURCE_DIR}/Bundle.cpp
	${CMAKE_SOURCE_DIR}/MiniPatch.cpp
	${CMAKE_SOURCE_DIR}/ShiTomasi.cpp
	${CMAKE_SOURCE_DIR}/SmallBlurryImage.cpp
	
	${CMAKE_SOURCE_DIR}/EyeGame.cpp	
	${CMAKE_SOURCE_DIR}/GLWindow2.cpp	
	${CMAKE_SOURCE_DIR}/GLWindowMenu.cpp
	${CMAKE_SOURCE_DIR}/VideoSource.cpp
	${CMAKE_SOURCE_DIR}/ATANCamera.cpp
	
	${CMAKE_SOURCE_DIR}/FAST/fast_7_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_7_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_8_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_8_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_9_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_9_score.cpp
        ${CMAKE_SOURCE_DIR}/FAST/fast_10_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_10_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_11_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_11_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_12_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_12_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/nonmax_suppression.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_corner.cpp
	
	${CMAKE_SOURCE_DIR}/GCVD/GLWindow.cpp
	${CMAKE_SOURCE_DIR}/GCVD/GLText.cpp
	${CMAKE_SOURCE_DIR}/GCVD/timer.cpp
	
	${CMAKE_SOURCE_DIR}/Persistence/PVars.cpp
	${CMAKE_SOURCE_DIR}/Persistence/instances.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI_language.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI_readline.cpp
	${CMAKE_SOURCE_DIR}/Persistence/serialize.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GStringUtil.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI_impl_readline.cpp
)


set(GPTAM_PROJ_INCLUDE
	${CMAKE_SOURCE_DIR}/OpenGL.h
	${CMAKE_SOURCE_DIR}/ARDriver.h
	${CMAKE_SOURCE_DIR}/System.h
	
	${CMAKE_SOURCE_DIR}/KeyFrame.h
	${CMAKE_SOURCE_DIR}/MapPoint.h
	${CMAKE_SOURCE_DIR}/Map.h
	${CMAKE_SOURCE_DIR}/MapViewer.h
	${CMAKE_SOURCE_DIR}/PatchFinder.h
	${CMAKE_SOURCE_DIR}/MapMaker.h
	${CMAKE_SOURCE_DIR}/LevelHelpers.h
	${CMAKE_SOURCE_DIR}/Tracker.h
	${CMAKE_SOURCE_DIR}/Relocaliser.h
	${CMAKE_SOURCE_DIR}/HomographyInit.h
	${CMAKE_SOURCE_DIR}/Bundle.h
	${CMAKE_SOURCE_DIR}/MiniPatch.h
	${CMAKE_SOURCE_DIR}/ShiTomasi.h
	${CMAKE_SOURCE_DIR}/SmallBlurryImage.h
	
	
	${CMAKE_SOURCE_DIR}/EyeGame.h
	${CMAKE_SOURCE_DIR}/GLWindow2.h
	${CMAKE_SOURCE_DIR}/GLWindowMenu.h
	${CMAKE_SOURCE_DIR}/VideoSource.h
	${CMAKE_SOURCE_DIR}/ATANCamera.h
	${CMAKE_SOURCE_DIR}/MEstimator.h
	
	${CMAKE_SOURCE_DIR}/FAST/prototypes.h
	${CMAKE_SOURCE_DIR}/FAST/nonmax_suppression.h
	${CMAKE_SOURCE_DIR}/FAST/fast_corner.h
	
	${CMAKE_SOURCE_DIR}/GCVD/Addedutils.h
	${CMAKE_SOURCE_DIR}/GCVD/image_interpolate.h
	${CMAKE_SOURCE_DIR}/GCVD/Operators.h
	${CMAKE_SOURCE_DIR}/GCVD/SO2.h
	${CMAKE_SOURCE_DIR}/GCVD/SO3.h
	${CMAKE_SOURCE_DIR}/GCVD/SE2.h
	${CMAKE_SOURCE_DIR}/GCVD/SE3.h
	${CMAKE_SOURCE_DIR}/GCVD/GLWindow.h
	${CMAKE_SOURCE_DIR}/GCVD/GLFont.h
	${CMAKE_SOURCE_DIR}/GCVD/GLHelpers.h
	${CMAKE_SOURCE_DIR}/GCVD/GraphSLAM.h
	${CMAKE_SOURCE_DIR}/GCVD/timer.h
	
	${CMAKE_SOURCE_DIR}/Persistence/default.h
	${CMAKE_SOURCE_DIR}/Persistence/serialize.h
	${CMAKE_SOURCE_DIR}/Persistence/type_name.h
	${CMAKE_SOURCE_DIR}/Persistence/PVars.h
	${CMAKE_SOURCE_DIR}/Persistence/GStringUtil.h
	${CMAKE_SOURCE_DIR}/Persistence/instances.h
	${CMAKE_SOURCE_DIR}/Persistence/GUI.h
	${CMAKE_SOURCE_DIR}/Persistence/GUI_impl.h
	${CMAKE_SOURCE_DIR}/Persistence/GUI_readline.h
	
     )


########## 2. CALIBRATOR SOURCE FILES  ###################
	

  
set(CALIB_PROJ_SOURCE
	${CMAKE_SOURCE_DIR}/CameraCalibrator.cpp
	${CMAKE_SOURCE_DIR}/GLWindow2.cpp	
	${CMAKE_SOURCE_DIR}/GLWindowMenu.cpp
	${CMAKE_SOURCE_DIR}/VideoSource.cpp
	${CMAKE_SOURCE_DIR}/CalibImage.cpp
	${CMAKE_SOURCE_DIR}/CalibCornerPatch.cpp
	${CMAKE_SOURCE_DIR}/ATANCamera.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_7_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_7_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_8_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_8_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_9_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_9_score.cpp
        ${CMAKE_SOURCE_DIR}/FAST/fast_10_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_10_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_11_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_11_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_12_detect.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_12_score.cpp
	${CMAKE_SOURCE_DIR}/FAST/nonmax_suppression.cpp
	${CMAKE_SOURCE_DIR}/FAST/fast_corner.cpp
	${CMAKE_SOURCE_DIR}/GCVD/GLWindow.cpp
	${CMAKE_SOURCE_DIR}/GCVD/GLText.cpp
	${CMAKE_SOURCE_DIR}/Persistence/PVars.cpp
	${CMAKE_SOURCE_DIR}/Persistence/instances.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI_language.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI_readline.cpp
	${CMAKE_SOURCE_DIR}/Persistence/serialize.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GStringUtil.cpp
	${CMAKE_SOURCE_DIR}/Persistence/GUI_impl_readline.cpp
)
set(CALIB_PROJ_INCLUDE
	
	${CMAKE_SOURCE_DIR}/GLWindow2.h
	${CMAKE_SOURCE_DIR}/GLWindowMenu.h
	${CMAKE_SOURCE_DIR}/VideoSource.h
	${CMAKE_SOURCE_DIR}/CalibImage.h
	${CMAKE_SOURCE_DIR}/CalibCornerPatch.h
	${CMAKE_SOURCE_DIR}/ATANCamera.h
	${CMAKE_SOURCE_DIR}/CameraCalibrator.h
	
	${CMAKE_SOURCE_DIR}/FAST/prototypes.h
	${CMAKE_SOURCE_DIR}/FAST/nonmax_suppression.h
	${CMAKE_SOURCE_DIR}/FAST/fast_corner.h
	
	${CMAKE_SOURCE_DIR}/GCVD/Addedutils.h
	${CMAKE_SOURCE_DIR}/GCVD/image_interpolate.h
	${CMAKE_SOURCE_DIR}/GCVD/Operators.h
	${CMAKE_SOURCE_DIR}/GCVD/SO2.h
	${CMAKE_SOURCE_DIR}/GCVD/SO3.h
	${CMAKE_SOURCE_DIR}/GCVD/SE3.h
	${CMAKE_SOURCE_DIR}/GCVD/GLWindow.h
	${CMAKE_SOURCE_DIR}/GCVD/GLFont.h
	${CMAKE_SOURCE_DIR}/GCVD/GLHelpers.h
	${CMAKE_SOURCE_DIR}/Persistence/default.h
	${CMAKE_SOURCE_DIR}/Persistence/serialize.h
	${CMAKE_SOURCE_DIR}/Persistence/type_name.h
	${CMAKE_SOURCE_DIR}/Persistence/PVars.h
	${CMAKE_SOURCE_DIR}/Persistence/GStringUtil.h
	${CMAKE_SOURCE_DIR}/Persistence/instances.h
	${CMAKE_SOURCE_DIR}/Persistence/GUI.h
	${CMAKE_SOURCE_DIR}/Persistence/GUI_impl.h
	${CMAKE_SOURCE_DIR}/Persistence/GUI_readline.h
	
     )
	
############## GUITEST SOURCE FILES DONE #########################



#Replaced bu emsripten options
## The notorious OpenGL linker flags
#SET( GL_LINKER_FLAGS "-lglut -lGLU -lGL -lglfw")
## More linker flags for the pthread and math
#SET( MISC_LINKER_FLAGS "-lpthread -lm")
## And another linker flag for the use of readline.h
#SET( GNU_READLINE_LINKER_FLAG "-lreadline")


# The gcalibrator executable
add_executable(${CALIB_PROJ_NAME}
	       ${CALIB_PROJ_SOURCE}
	       ${CALIB_PROJ_INCLUDE}
              )

target_link_libraries(${CALIB_PROJ_NAME}
		      ${EXT_LIBS}
#		      ${GL_LINKER_FLAGS}
#		      ${GNU_READLINE_LINKER_FLAG}
#		      ${MISC_LINKER_FLAGS}
		      )

target_include_directories(${CALIB_PROJ_NAME} PRIVATE "${EXT_INCLUDE_DIRS}")

      
		      
# The gptam executable
add_executable(${GPTAM_PROJ_NAME}
	      ${GPTAM_PROJ_SOURCE}
	      ${GPTAM_PROJ_INCLUDE}
              )

target_link_libraries(${GPTAM_PROJ_NAME}
		      ${EXT_LIBS}
#		      ${GL_LINKER_FLAGS}
#		      ${GNU_READLINE_LINKER_FLAG}
#		      ${MISC_LINKER_FLAGS}
		      )

target_include_directories(${GPTAM_PROJ_NAME} PRIVATE "${EXT_INCLUDE_DIRS}")

############ DEFINING NECESSARY MACROS (Use_XMMINTRIN=1 for now...) ###############
set(EMCC_LINKER_FLAGS "--emrun -O2 -s WASM=1 \
                       --preload-file ../calibrator_settings.cfg@/calibrator_settings.cfg \
                       --preload-file ../settings.cfg@/settings.cfg \
                       -s USE_WEBGL2=1 -s FULL_ES3=1 -s LEGACY_GL_EMULATION=1 -s GL_FFP_ONLY=1 -s USE_GLFW=3 \
                       -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s EMTERPRETIFY_ASYNC=1") # -s USE_PTHREADS=1 PROXY_TO_WORKER
#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${EMCC_LINKER_FLAGS})
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${EMCC_LINKER_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS ${EMCC_LINKER_FLAGS})
set(CMAKE_EXECUTABLE_SUFFIX ".html")

target_compile_definitions(${GPTAM_PROJ_NAME} PRIVATE USE_XMMINTRIN=0)
remove_definitions(-WIN32)	

TARGET_COMPILE_OPTIONS(${CALIB_PROJ_NAME} PRIVATE "-D_LINUX -Wall")
TARGET_COMPILE_OPTIONS(${GPTAM_PROJ_NAME} PRIVATE "-D_LINUX -Wall")


ADD_EXECUTABLE(maintest maintest.cpp)
TARGET_LINK_LIBRARIES(maintest ${EXT_LIBS})
TARGET_COMPILE_OPTIONS(maintest PRIVATE "-D_LINUX -Wall")

SET_TARGET_PROPERTIES(${CALIB_PROJ_NAME} ${GPTAM_PROJ_NAME} maintest
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        )
