cmake_minimum_required(VERSION 2.6)

set(GPTAM_PROJ_NAME gptam)
set(CALIB_PROJ_NAME gcalibrator)

set(CMAKE_CXX_STANDARD 14)



######################################
# The external reference directory (searching) for cmake
set(EXT_DIR ${CMAKE_SOURCE_DIR}/cmake )
list( APPEND CMAKE_MODULE_PATH
  ${EXT_DIR}
)

## find openCV
set(OpenCV_STATIC ON)
set(OpenCV_DIR /opt/opencv/build_js)
find_package(OpenCV REQUIRED)


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




# The notorious OpenGL linker flags
SET( GL_LINKER_FLAGS "-lglut -lGLU -lGL")
# More linker flags for the pthread and math (had to look this one up and just copied it)
SET( PTHREAD_PROBLEM_LINKER_FLAGS "-lpthread -lm")
# And another linker flag for the use of readline.h
SET( GNU_READLINE_LINKER_FLAG "-lreadline")

# declaring external library include directories
include_directories(${EXT_INCLUDE_DIRS})

# The gcalibrator executable     
add_executable(${CALIB_PROJ_NAME}
	       ${CALIB_PROJ_SOURCE}
	       ${CALIB_PROJ_INCLUDE}
              )
        

              
target_link_libraries(${CALIB_PROJ_NAME}
		      ${EXT_LIBS}
		      ${GL_LINKER_FLAGS}
		      ${PTHREAD_PROBLEM_LINKER_FLAGS}
		      ${GNU_READLINE_LINKER_FLAG}
		      )

        
install(TARGETS ${CALIB_PROJ_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})

      
		      
# The gptam executable
add_executable(${GPTAM_PROJ_NAME}
	      ${GPTAM_PROJ_SOURCE}
	      ${GPTAM_PROJ_INCLUDE}
              )



target_link_libraries(${GPTAM_PROJ_NAME}
		      ${EXT_LIBS}
		      ${GL_LINKER_FLAGS}
		      ${PTHREAD_PROBLEM_LINKER_FLAGS}
		      ${GNU_READLINE_LINKER_FLAG}
		      )

############ DEFINING NECESSARY MACROS (Use_XMMINTRIN=1 for now...) ###############
set(EMCC_LINKER_FLAGS "--bind -s WASM=1 -O3 -s USE_WEBGL2=1 -s GL_STATE_CACHE=1 -s USE_GLFW=3")
set(CMAKE_REQUIRED_FLAGS "${EMCC_LINKER_FLAGS}")

target_compile_definitions(${GPTAM_PROJ_NAME} PRIVATE USE_XMMINTRIN=0)
remove_definitions(-WIN32)	

set_property(TARGET ${CALIB_PROJ_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "-D_LINUX -Wall")
set_property(TARGET ${GPTAM_PROJ_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "-D_LINUX -Wall")
#install(TARGETS ${GPTAM_PROJ_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
set_target_properties( ${CALIB_PROJ_NAME} ${GPTAM_PROJ_NAME}
        PROPERTIES
        LINK_FLAGS "${EMCC_LINKER_FLAGS}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        )

SET(CMAKE_CACHEFILE_DIR "cmake-build-debug")
