#include "GLWindow.h"
#include <exception>


GLXInterface::Exceptions::GLWindow::CreationError::CreationError(std::string w)
{
    //what() = "GLWindow creation error: " + w;
}

GLXInterface::Exceptions::GLWindow::RuntimeError::RuntimeError(std::string w)
{
    //std::string str = 
    //what() = "GLWindow error: " + w;
}

GLXInterface::GLWindow::GLWindow(const cv::Size2i& size, const std::string& title, GLFWmousebuttonfun mouse_button_callback,
                                 GLFWcursorposfun cursor_position_callback, GLFWkeyfun key_callback, GLFWscrollfun scroll_callback,
                                 GLFWwindowclosefun close_callback, GLFWwindowsizefun size_callback, int bpp){
	init(size, bpp, title, mouse_button_callback, cursor_position_callback, key_callback, scroll_callback, close_callback, size_callback);
}

GLXInterface::GLWindow::GLWindow(const cv::Size2i& size, GLFWmousebuttonfun mouse_button_callback,
              GLFWcursorposfun cursor_position_callback, GLFWkeyfun key_callback, GLFWscrollfun scroll_callback,
              GLFWwindowclosefun close_callback, GLFWwindowsizefun size_callback, int bpp, const std::string& title){
	init(size, bpp, title, mouse_button_callback, cursor_position_callback, key_callback, scroll_callback, close_callback, size_callback);
}

void GLXInterface::GLWindow::init(const cv::Size2i& size, int bpp, const std::string& title, GLFWmousebuttonfun mouse_button_callback,
                                  GLFWcursorposfun cursor_position_callback, GLFWkeyfun key_callback, GLFWscrollfun scroll_callback,
                                  GLFWwindowclosefun close_callback, GLFWwindowsizefun size_callback)
{
    glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		throw Exceptions::GLWindow::CreationError("Cannot init GLFW");
	}
	GLFWwindow* window = glfwCreateWindow(size.width, size.height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		throw Exceptions::GLWindow::CreationError("Window couldn't be created");
	}

    set_events(mouse_button_callback, cursor_position_callback, key_callback, scroll_callback, close_callback, size_callback);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, bpp/3);
	glfwWindowHint(GLFW_GREEN_BITS, bpp/3);
	glfwWindowHint(GLFW_BLUE_BITS, bpp/3);
	glfwWindowHint(GLFW_ALPHA_BITS, bpp/3);
	glfwWindowHint(GLFW_DEPTH_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);

	glfwMakeContextCurrent(window);

    glLoadIdentity();
    glViewport(0, 0, size.width, size.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(-1, 1);
    glOrtho(-0.375, size.width-0.375, size.height-0.375, -0.375, -1 , 1); //offsets to make (0,0) the top left pixel (rather than off the display)
    glPixelZoom(1,-1);

    state = new State();
    state->title = title;
    state->window = window;
}

GLXInterface::GLWindow::~GLWindow()
{
	glfwTerminate();
	glfwDestroyWindow(state->window);
    delete state;
}

cv::Size2i GLXInterface::GLWindow::size() const {
    cv::Point2i where;
    glfwGetWindowSize(state->window, &where.x, &where.y);
    return where;
}

void GLXInterface::GLWindow::set_size(const cv::Size2i &s_){
	glfwSetWindowSize(state->window, s_.width, s_.height);
}

cv::Point2i GLXInterface::GLWindow::position() const {
    cv::Point2i where;
    glfwGetWindowPos(state->window, &where.x, &where.y);
    return where;
}

void GLXInterface::GLWindow::set_position(const cv::Point2i &p_){
	glfwSetWindowPos(state->window, p_.x, p_.y);
}

void GLXInterface::GLWindow::set_cursor_position(const cv::Point2i &where)
{
    glfwSetCursorPos(state->window, where.x, where.y);
}

cv::Point2i GLXInterface::GLWindow::cursor_position() const
{
    double x, y;
    glfwGetCursorPos(state->window, &x, &y);
    return cv::Point2i((int)x, (int)y);
}

void GLXInterface::GLWindow::show_cursor(bool show)
{
    if (show)
        glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

std::string GLXInterface::GLWindow::title() const
{
    return state->title;
}

void GLXInterface::GLWindow::swap_buffers()
{
    glfwSwapBuffers(state->window);
}

void GLXInterface::GLWindow::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void GLXInterface::GLWindow::set_events(GLFWmousebuttonfun mouse_button_callback, GLFWcursorposfun cursor_position_callback,
                                        GLFWkeyfun key_callback, GLFWscrollfun scroll_callback,
                                        GLFWwindowclosefun close_callback, GLFWwindowsizefun size_callback){
    glfwSetMouseButtonCallback(state->window, mouse_button_callback);
    glfwSetCursorPosCallback(state->window, cursor_position_callback);
    glfwSetKeyCallback(state->window, key_callback);
    glfwSetScrollCallback(state->window, scroll_callback);
    glfwSetWindowCloseCallback(state->window, close_callback);
    glfwSetWindowSizeCallback(state->window, size_callback);
}

void GLXInterface::GLWindow::handle_events()
{
    glfwPollEvents();
}

void GLXInterface::GLWindow::MakeSummary::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cv::Point2i where((int)x, (int)y);
    switch(action){
        case GLFW_PRESS:
            summary.mouse_down[button] = std::make_pair(where, button);
            break;
        case GLFW_RELEASE:
            summary.mouse_up[button] = std::make_pair(where, button);
            break;
    }
}

void GLXInterface::GLWindow::MakeSummary::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    summary.cursor = cv::Point2i(xpos, ypos);
    summary.cursor_moved = true;
}

void GLXInterface::GLWindow::MakeSummary::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch(action){
        case GLFW_PRESS:
            ++summary.key_down[key];
            break;
        case GLFW_RELEASE:
            ++summary.key_up[key];
            break;
    }
}


void GLXInterface::GLWindow::activate()
{
    glfwMakeContextCurrent(state->window);
}
