// George Terzakis 2016
//
// University of Portsmouth
//
// Code based on PTAM by Klein and Murray (Copyright 2008 Isis Innovation Limited)

#include "OpenGL.h"
#include "GLWindow2.h"
#include "GLWindowMenu.h"
#include "GCVD/GLHelpers.h"

#include <stdlib.h>
#include "Persistence/GStringUtil.h"
#include "Persistence/instances.h"


using namespace std;
using namespace GLXInterface;
using namespace Persistence;

#include <GL/gl.h>
#include <GL/glut.h>

std::vector<GLWindowMenu*> GLWindow2::mvpGLWindowMenus;
cv::Point2i GLWindow2::mirLastMousePos;
cv::Vec<float, 6> GLWindow2::mvMCPoseUpdate;
cv::Vec<float, 6> GLWindow2::mvLeftPoseUpdate;

GLWindow2::GLWindow2(cv::Size2i irSize, string sTitle) : GLWindow(irSize, sTitle, GLWindow2::mouse_button_callback,
                                                                  GLWindow2::cursor_position_callback, GLWindow2::key_callback,
                                                                  GLWindow2::scroll_callback, GLWindow2::close_callback)
{

#ifdef WIN32
  // On windows, have to initialise GLEW at the start to enable access
  // to GL extensions
  static bool bGLEWIsInit = false;
  if(!bGLEWIsInit)
  {
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW Error:  " << glewGetErrorString(err) << std::endl;;
		exit(0);
	}
	bGLEWIsInit = true;
  }
#endif

  mirVideoSize = irSize;
  GUI.RegisterCommand("GLWindow.AddMenu", GUICommandCallBack, this);
  glSetFont("sans");
  mvMCPoseUpdate = cv::Vec<float, 6>(0, 0, 0, 0, 0 , 0);
  mvLeftPoseUpdate = cv::Vec<float, 6>(0, 0, 0, 0, 0 , 0);
};


void GLWindow2::AddMenu(string sName, string sTitle)
{
  GLWindowMenu* pMenu = new GLWindowMenu(sName, sTitle); 
  mvpGLWindowMenus.push_back(pMenu);
}

void GLWindow2::GUICommandCallBack(void* ptr, string sCommand, string sParams)
{
  ((GLWindow2*) ptr)->GUICommandHandler(sCommand, sParams);
}

void GLWindow2::GUICommandHandler(string sCommand, string sParams)  // Called by the callback func..
{
  vector<string> vs = ChopAndUnquoteString(sParams);
  if(sCommand=="GLWindow.AddMenu")
    {
      switch(vs.size())
	{
	case 1:
	  AddMenu(vs[0], "Root");
	  return;
	case 2:
	  AddMenu(vs[0], vs[1]);
	  return;
	default:
	  cout << "? AddMenu: need one or two params (internal menu name, [caption])." << endl;
	  return;
	};
    };
  
  // Should have returned to caller by now - if got here, a command which 
  // was not handled was registered....
  cout << "! GLWindow::GUICommandHandler: unhandled command "<< sCommand << endl;
  exit(1);
}; 

void GLWindow2::DrawMenus()
{
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_RECTANGLE_ARB);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_POLYGON_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColorMask(1,1,1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  SetupWindowOrtho();
  glLineWidth(1);
  
  int nTop = 30;
  int nHeight = 30;
  for(vector<GLWindowMenu*>::iterator i = mvpGLWindowMenus.begin(); i!= mvpGLWindowMenus.end(); i++) {
    
      (*i)->Render(nTop, nHeight, size().width, *this);
      nTop+=nHeight+1;
    }
  
}

// STOP THE PRESS!!!! 
// etting up unit ortho 
void GLWindow2::SetupUnitOrtho()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,1,1,0,0,1);
}

void GLWindow2::SetupWindowOrtho()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLXInterface::glOrtho(size());
  
}

// STOP THE PRESS!!!! This glOrtho configuration is used during the calibration
// and it also described the ATANCamera projection/unprojection functions...
// The 0.5 margin given as a made-up additional minor offset, which -most probably- is Klein's way 
// of preventing border image points from falling off the visible OpenGL canvas.
/// We setup a window coordinate system as (-0.5 to -0.5 + IMG_WIDTH) x (-0.5 + IMG_HEIGHT, -0.5). In other words, we draw driectly in image coordinates
void GLWindow2::SetupVideoOrtho()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5,(double)mirVideoSize.width - 0.5, (double)mirVideoSize.height - 0.5, -0.5, -1.0, 1.0);
}

void GLWindow2::SetupVideoRasterPosAndZoom()
{ 
  glRasterPos2d(-0.5,-0.5);
  float adZoom[2];
  adZoom[0] = (float) size().width / (float) mirVideoSize.width;
  adZoom[1] = (float) size().height / (float) mirVideoSize.height;
  glPixelZoom(adZoom[0], -adZoom[1]);
}

void GLWindow2::SetupViewport()
{
  glViewport(0, 0, size().width, size().height);
}

void GLWindow2::PrintString(cv::Point2i irPos, std::string s)
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glTranslatef(irPos.x, irPos.y, 0.0);
  glScalef(8,-8,1);
  GLXInterface::glDrawText(s, GLXInterface::NICE, 1.6, 0.1);
  glPopMatrix();
}

void GLWindow2::DrawCaption(string s)
{
  if(s.length() == 0) return;
  
  SetupWindowOrtho();
  // Find out how many lines are in the caption:
  // Count the endls
  int nLines = 0;
  {
    string sendl("\n");
    string::size_type st=0;
    while(1) {
	nLines++;
	st = s.find(sendl, st);
	if(st==string::npos)  break;
	else
	  st++;
    }
  }
  
  int nTopOfBox = size().height - nLines * 17;
  
  // Draw a grey background box for the text
  glColor4f(0,0,0,0.4);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_QUADS);
  glVertex2d(-0.5, nTopOfBox);
  glVertex2d(size().width, nTopOfBox);
  glVertex2d(size().width, size().height);
  glVertex2d(-0.5, size().height);
  glEnd();
  
  // Draw the caption text in yellow
  glColor3f(1,1,0);      
  PrintString(cv::Point2i(10, nTopOfBox + 13), s);
}


void GLWindow2::HandlePendingEvents()
{
  handle_events();
}

void GLWindow2::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    cv::Point2i where(xpos, ypos);
    cv::Point2i irMotion = where - mirLastMousePos;
    mirLastMousePos = where;

    double dSensitivity = 0.01;

    bool middleButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
    bool leftButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool rightButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (middleButtonPressed || (rightButtonPressed && leftButtonPressed)) {
      mvLeftPoseUpdate[5] -= irMotion.x * dSensitivity;
      mvLeftPoseUpdate[2] += irMotion.y * dSensitivity;
    }
    else if (leftButtonPressed){
      mvMCPoseUpdate[3] -= irMotion.y * dSensitivity;
      mvMCPoseUpdate[4] += irMotion.x * dSensitivity;
    }
    else if (rightButtonPressed) {
      mvLeftPoseUpdate[4] -= irMotion.x * dSensitivity;
      mvLeftPoseUpdate[3] += irMotion.y * dSensitivity;
    }
}

void GLWindow2::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;
    bool bHandled = false;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    for(unsigned int i=0; !bHandled && i<mvpGLWindowMenus.size(); i++)
        bHandled = mvpGLWindowMenus[i]->HandleClick(button, x, y);

}

void GLWindow2::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    bool bHandled = false;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cv::Point2i where((int)x, (int)y);

    for(unsigned int i=0; !bHandled && i<mvpGLWindowMenus.size(); i++)
        bHandled = mvpGLWindowMenus[i]->HandleScroll(xoffset, yoffset, where.x, where.y);
}

pair<cv::Vec<float, 6>, cv::Vec<float, 6> > GLWindow2::GetMousePoseUpdate()
{
  pair<cv::Vec<float, 6>, cv::Vec<float, 6> > result = make_pair(mvLeftPoseUpdate, mvMCPoseUpdate);
  mvLeftPoseUpdate = cv::Vec<float, 6>(0, 0, 0, 0, 0, 0); 
  mvMCPoseUpdate = cv::Vec<float, 6>(0, 0, 0, 0, 0, 0);
  
  return result;
}

void GLWindow2::close_callback(GLFWwindow * window){
    GUI.ParseLine("quit");
}

void GLWindow2::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    string s;
    const char *c = glfwGetKeyName(key, 0);

    if (c != nullptr)
        s = c;
    else switch(key){
        case GLFW_KEY_PAGE_UP:
            s="PageUp";
            break;
        case GLFW_KEY_PAGE_DOWN:
            s="PageDown";
            break;
        case GLFW_KEY_ENTER:
            s="Enter";
            break;
        case GLFW_KEY_SPACE:
            s="Space";
            break;
        case GLFW_KEY_BACKSPACE:
            s="BackSpace";
            break;
        case GLFW_KEY_ESCAPE:
            s="Escape";
            break;
    }

    if(s != "") GUI.ParseLine("try KeyPress "+s);
}

