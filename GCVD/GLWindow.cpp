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

GLXInterface::GLWindow::GLWindow(const cv::Size2i& size, const std::string& title, EventHandler& handler, int bpp){
	init(size, bpp, title, handler);
}

GLXInterface::GLWindow::GLWindow(const cv::Size2i& size, EventHandler& handler, int bpp, const std::string& title){
	init(size, bpp, title, handler);
}

void GLXInterface::GLWindow::init(const cv::Size2i& size, int bpp, const std::string& title, EventHandler& handler)
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

    set_events(handler);

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
    glfwGetFramebufferSize(window, &size.width, &size.height);
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
	glfwDestroyWindow(state.window);
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
    
    state->position = p_;
	glfwSetWindowPos(state->window, p_.x, p_.y);
}

void GLXInterface::GLWindow::set_cursor_position(const cv::Point2i &where)
{
    glfwSetCursorPos(state->window, where.x, where.y);
}

cv::Point2i GLXInterface::GLWindow::cursor_position() const
{
    cv::Point2i where;
    glfwGetCursorPos(state->window, &where.x, &where.y);
    return where;
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


void GLXInterface::GLWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    //TODO
}

static void GLXInterface::GLWindow::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    //TODO
}

void GLXInterface::GLWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    //TODO
}

void GLXInterface::GLWindow::set_events(EventHandler& handler){
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetKeyCallback(window, key_callback);

//	switch (event.type) {
//	case ButtonPress:
//	    handler.on_mouse_down(*this, cv::Point2i(event.xbutton.x, event.xbutton.y),
//				  convertButtonState(event.xbutton.state), convertButton(event.xbutton.button));
//	    break;
//	case ButtonRelease:
//	    handler.on_mouse_up(*this, cv::Point2i(event.xbutton.x, event.xbutton.y),
//				convertButtonState(event.xbutton.state), convertButton(event.xbutton.button));
//	    break;
//	case MotionNotify:
//	    handler.on_mouse_move(*this, cv::Point2i(event.xmotion.x, event.xmotion.y), convertButtonState(event.xbutton.state));
//	    break;
//	case KeyPress:
//		{
//		XLookupString(&event.xkey, 0, 0, &k, 0);
//	    handler.on_key_down(*this, k);
//	    break;
//		}
//	case KeyRelease:
//		XLookupString(&event.xkey, 0, 0, &k, 0);
//	    handler.on_key_up(*this, k);
//	    break;
//	    //case UnmapNotify: active = 0; break;
//	    //case MapNotify: active = 1; break;
//	case ConfigureNotify:
//	    if (event.xconfigure.width != state->size.width || event.xconfigure.height != state->size.height) {
//		activate();
//		state->size = cv::Size2i(event.xconfigure.width, event.xconfigure.height);
//		glViewport(0, 0, state->size.width, state->size.height);
//		//glRasterPos2f(0,0);
//		//glPixelZoom(float(event.xconfigure.width)/myWidth,-float(event.xconfigure.height)/myHeight);
//		handler.on_resize(*this, state->size);
//	    }
//	    break;
//	case Expose:
//		handler.on_event(*this, EVENT_EXPOSE);
//		break;
//	case ClientMessage:
//	    if (event.xclient.data.l[0] == (int)state->delete_atom)
//		handler.on_event(*this, EVENT_CLOSE);
//	    else
//		handler.on_event(*this, event.xclient.message_type);
//	    break;
//	default:
//	    handler.on_event(*this, event.type);
//	    break;
//	}
//    }
}

void GLXInterface::GLWindow::handle_events()
{
    glfwPollEvents();
}


class MakeSummary : public GLXInterface::GLWindow::EventHandler {
private:
    GLXInterface::GLWindow::EventSummary& summary;
public:
    MakeSummary(GLXInterface::GLWindow::EventSummary& summary_) : summary(summary_) {}

    void on_key_down(GLXInterface::GLWindow&, int key) {	++summary.key_down[key]; }
    void on_key_up(GLXInterface::GLWindow&, int key) { ++summary.key_up[key]; }
    void on_mouse_move(GLXInterface::GLWindow&, cv::Point2i where, int) { summary.cursor = where; summary.cursor_moved = true; }
    void on_mouse_down(GLXInterface::GLWindow&, cv::Point2i where, int state, int button) { summary.mouse_down[button] = std::make_pair(where,state); }
    void on_mouse_up(GLXInterface::GLWindow&, cv::Point2i where, int state, int button) { summary.mouse_up[button] = std::make_pair(where,state); }
    void on_event(GLXInterface::GLWindow&, int event) { ++summary.events[event]; }
};


void GLXInterface::GLWindow::activate()
{
    glfwMakeContextCurrent(state->window);
}
