// George Terzakis 2016
//
// University of Portsmouth
//
// Code based on PTAM by Klein and Murray (Copyright 2008 Isis Innovation Limited)

#ifndef __GL_WINDOW_2_H
#define __GL_WINDOW_2_H
//
//  A class which wraps a CVD::GLWindow and provides some basic
//  user-interface funtionality: A gvars-driven clickable menu, and a
//  caption line for text display. Also provides some handy GL helpers
//  and a wrapper for CVD's text display routines.

//#include <cvd/glwindow.h>
//#include <TooN/TooN.h>

#include <opencv2/core.hpp>
#include <cv.hpp>
#include <opencv2/highgui.hpp>

#include <GL/gl.h>
#include <GL/glut.h>


#include "GCVD/GLWindow.h"

//#include "GLWindowMenu.h"


using namespace GLXInterface;

// forward declaration to resolve cyclic references (still the darn thing complains...)
class GLWindowMenu;

enum GUICommand { ccmd_GrabNextFrame,
			    ccmd_Reset,
			    ccmd_ShowNext,
			    ccmd_SaveCalib,
			    ccmd_Quit,
			    ccmd_Optimize,
			    ccmd_ShowGrabbedFrame,
			    ccmd_ToggleNoDist,
			    ccmd_Exit };


//class GLWindow2 : public CVD::GLWindow
class GLWindow2 : public GLWindow
{

public:
  
  
  
  GLWindow2(cv::Size2i irSize, std::string sTitle);
  
  // The preferred event handler..
  void HandlePendingEvents();
  
  // Menu interface:
  void AddMenu(std::string sName, std::string sTitle);
  void DrawMenus();
  
  // Some OpenGL helpers:
  void SetupViewport();
  void SetupVideoOrtho();
  void SetupUnitOrtho();
  void SetupWindowOrtho();
  void SetupVideoRasterPosAndZoom();

  // Text display functions:
  void PrintString(cv::Point2i irPos, std::string s);
  void DrawCaption(std::string s);
  
  // Map viewer mouse interface:
  std::pair< cv::Vec<float, 6>, cv::Vec<float, 6> > GetMousePoseUpdate();
  
  void addMyMenus();
  

protected:
  void GUICommandHandler(std::string sCommand, std::string sParams);
  static void GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams);
  
  
  // User interface menus:
  static std::vector<GLWindowMenu*> mvpGLWindowMenus;

  cv::Size2i mirVideoSize;   // The size of the source video material.
  

  // Event handling routines:
  static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
  static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void close_callback(GLFWwindow * window);
  static cv::Point2i mirLastMousePos;

  // Storage for map viewer updates:
  static cv::Vec<float, 6> mvMCPoseUpdate;
  static cv::Vec<float, 6> mvLeftPoseUpdate;
  

};








#endif
