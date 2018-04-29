#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <string>
#include <vector>
#include <map>

#include "../OpenCV.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
//#include <GL/glfw.h>
#include <GLFW/glfw3.h>

namespace GLXInterface {

    namespace Exceptions
    {
	/// %Exceptions specific to CVD::GLWindow
	/// @ingroup gException
	namespace GLWindow
	{
	    /// Base class for all CVD::GLWindow exceptions
	    /// @ingroup gException
	    //struct All: public CVD::Exceptions::All{
	    //};

	    struct All: public std::exception {
	    
	    };
	      
	    /// An exception occurred during initialisation
	    struct CreationError: public All
	    {
		CreationError(std::string w); ///< Construct from error string
	    };

	    /// An exception occurred during run-time
	    struct RuntimeError: public All
	    {
		RuntimeError(std::string w); ///< Construct from error string
	    };
	}
    }

    /// An object that creates a window and a GL context attached to that window, and manages its events.
    class GLWindow {

	public:

	/// Abstract base class for event handlers.  Subclass this and override to implement a handler.
	class EventHandler {
    private:
        EventHandler(){}
    public:
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){}
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){}
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){}
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){}
        static void close_callback(GLFWwindow *){}
		static void size_callback(GLFWwindow *, int, int){}
	};

	/// A summary of multiple events
	struct EventSummary {

	    EventSummary() : cursor(-1,-1), cursor_moved(false) {}
	    /// key->frequency mapping for key presses and releases
	    std::map<int,int> key_down, key_up;
	    typedef std::map<int,int>::const_iterator key_iterator;
	    /// button->frequency mapping for mouse presses and releases
	    std::map<int,std::pair<cv::Point2i, int> > mouse_down, mouse_up;
	    typedef std::map<int,std::pair<cv::Point2i,int>>::const_iterator mouse_iterator;
	    /// Reset the summary
	    void clear() { *this = EventSummary(); }
	    /// last seen cursor position from mouse_move
	    cv::Point2i cursor;
	    /// was the cursor moved during the recording of the history
	    bool cursor_moved;
	};

    class MakeSummary {
    private:
        MakeSummary(){}
        static GLXInterface::GLWindow::EventSummary summary;
    public:
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };

	/// Construct a GLWindow of the given size and colour depth, with the given title.
	/// A double-buffered GL context is associated with the window.
	/// @param size    Window size
	/// @param bpp     Colour depth
	/// @param title   Window title

	GLWindow(const cv::Size2i& size,
             GLFWmousebuttonfun mouse_button_callback = MakeSummary::mouse_button_callback,
             GLFWcursorposfun cursor_position_callback = MakeSummary::cursor_position_callback,
             GLFWkeyfun key_callback = MakeSummary::key_callback,
             GLFWscrollfun scroll_callback = EventHandler::scroll_callback,
             GLFWwindowclosefun close_callback = EventHandler::close_callback,
             GLFWwindowsizefun size_callback = EventHandler::size_callback,
             int bpp=24, const std::string& title="GLWindow");

	///@overload
	GLWindow(const cv::Size2i& size, const std::string& title,
             GLFWmousebuttonfun mouse_button_callback = MakeSummary::mouse_button_callback,
             GLFWcursorposfun cursor_position_callback = MakeSummary::cursor_position_callback,
             GLFWkeyfun key_callback = MakeSummary::key_callback,
             GLFWscrollfun scroll_callback = EventHandler::scroll_callback,
             GLFWwindowclosefun close_callback = EventHandler::close_callback,
             GLFWwindowsizefun size_callback = EventHandler::size_callback,
             int bpp=24);

	~GLWindow();
	/// Get the size
	cv::Size2i size() const;
	/// Set the size
	void set_size(const cv::Size2i &);
	/// Get the position
	cv::Point2i position() const;
	/// Set the position
	void set_position(const cv::Point2i &);
	/// Set the mouse cursor position
	void set_cursor_position(const cv::Point2i& where);
	/// Get the mouse cursor position
	cv::Point2i cursor_position() const;
	/// Show (or hide) the cursor
	void show_cursor(bool show=true);
	/// Hide the cursor
	void hide_cursor() { show_cursor(false); }
	/// Get the title
	std::string title() const;
	/// Swap the front and back buffers
	void swap_buffers();
	/// register event handlers
	void set_events(
            GLFWmousebuttonfun mouse_button_callback,
            GLFWcursorposfun cursor_position_callback,
            GLFWkeyfun key_callback,
            GLFWscrollfun scroll_callback,
            GLFWwindowclosefun close_callback,
            GLFWwindowsizefun size_callback);
	/// Handle events in the event queue by calling back to the specified handler.
	void handle_events();
	/// Make this GL context active
	void activate();
	/// Make this GL context active
	void make_current() { activate(); }

	/// events handler
    static void error_callback(int error, const char* description);
    
    struct State {
        std::string title;
        GLFWwindow* window;
    };
	   
    private:
	State* state;
	void init(const cv::Size2i& sz, int bpp, const std::string& title,
              GLFWmousebuttonfun mouse_button_callback,
              GLFWcursorposfun cursor_position_callback,
              GLFWkeyfun key_callback,
              GLFWscrollfun scroll_callback,
              GLFWwindowclosefun close_callback,
              GLFWwindowsizefun size_callback);
    
	
    };
}


#endif