#pragma once

namespace glutil{

	/**
	*	@brief interface for call backs.
	*	A class that wishes to register for callbacks must subclass this interface.
	*	The default implementations are empty and not pure virtual to avoid forcing
	*	the subclasses implement the callbacks they do not need.
	*/
	class CallbackListener{
	public:
		virtual ~CallbackListener(){}
		virtual void mouseButtonCallback(GLFWwindow*, int, int, int){}
		virtual void mouseScrollCallback(GLFWwindow*, double, double){}
		virtual void mousePositionCallback(GLFWwindow*, double, double){}
		virtual void keyCallback(GLFWwindow*, int, int, int, int){}
	};

	class WindowHandler
	{
	public:
		typedef unsigned int CallbackID;
		static const CallbackID MouseButtonCallbackID = 0;
		static const CallbackID MouseScrollCallbackID = 1;
		static const CallbackID MousePositionCallbackID = 2;
		static const CallbackID KeyCallbackID = 3;

		WindowHandler();
		WindowHandler(const std::string& windowName, bool allowResize = true,
			GLuint wWidth = 800, GLuint wHeight = 800);
		WindowHandler(const WindowHandler&) = delete;
		WindowHandler& operator=(const WindowHandler&) = delete;
		virtual ~WindowHandler();

		/**
		*	Initializes the window.
		*	@throws std::exception if the initialization fails
		*/
		virtual void initialize();
		/**
		*	@brief Will call the glfw method to find out if the winder hsa received the close command
		*	@return whether the window should close
		*/
		virtual int shouldClose() const;
		/**
		*	@brief Directly set the close value of the window
		*	@param close whether the window should close
		*/
		inline void setShouldClose(bool close) const;

		/**
		*	Swaps the buffers
		*/
		inline void swapBuffers() const;

		/**
		*	@brief Register a callbacklistener to a particular callback
		*	@param listener a subclass of CallbackListener
		*	@param id a valid callback ID as defined in WindowHandler.h
		*/
		void registerForCallback(CallbackListener* listener, CallbackID id);

		/**
		*	@brief Unregister a callbacklistener from a particular callback
		*	Note. It is the responsibility of the listeners to properly unregister themselves
		*	before they are destroyed. Otherwise, it is possible that an event callback will be called
		*	on a dangling pointer.
		*	@param listener a subclass of CallbackListener
		*	@param id a valid callback ID as defined in WindowHandler.h
		*/
		void unregisterForCallback(CallbackListener* listener, CallbackID id);

		/**
		*	@brief find out if the window is initialized
		*	@return true if the window is already initialized, false otherwise
		*/
		inline bool isInitialized() const;

		inline bool getAllowWindowResize() const;

		/**
		*	@brief tell the windowhandler if it should handle the window resize
		*	The WindowHandler will attempt to adjust the ViewPort only if it is allowed to.
		*	@param allow whether the WindowHandler is allowed to handle window resizes
		*/
		inline void setHandleWindowSizeCallback(bool allow);
		inline bool getHandleWindowSizeCallback() const;

		inline void setWindowHeight(GLuint wHeight);
		inline GLuint getWindowHeight() const;

		inline void setWindowWidth(GLuint wWidth);
		inline GLuint getWindowWidth() const;

		const std::string windowName;
		const bool allowWindowResize;
	protected:
		std::map<CallbackID, std::set<CallbackListener*>> listenerMap;

		bool handleWindowSizeCallback;
		bool initialized;
		GLuint windowWidth;
		GLuint windowHeight;
		GLFWwindow* window;

		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
		void mousePoistionCallback(GLFWwindow* window, double xpos, double ypos);
		void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		virtual void windowSizeCallback(GLFWwindow* window, int width, int height);

		inline static void s_keyCallback(GLFWwindow *win, int key, int scancode, int action, int mode) {
			WindowHandler *handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(win));
			handler->keyCallback(win, key, scancode, action, mode);
		}

		inline static void s_mousePositionCallback(GLFWwindow *win, double xpos, double ypos) {
			WindowHandler *handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(win));
			handler->mousePoistionCallback(win, xpos, ypos);
		}

		inline static void s_mouseScrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
			WindowHandler *handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(win));
			handler->mouseScrollCallback(win, xoffset, yoffset);
		}

		inline static void s_mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
			WindowHandler *handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(win));
			handler->mouseButtonCallback(win, button, action, mods);
		}

		inline static void s_windowResizeCallback(GLFWwindow *win, int w, int h) {
			WindowHandler *handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(win));
			handler->windowSizeCallback(win, w, h);
		}
	};

	bool WindowHandler::isInitialized() const
	{
		return initialized;
	}

	void WindowHandler::swapBuffers() const
	{
		glfwSwapBuffers(window);
	}

	void WindowHandler::setShouldClose(bool close) const
	{
		glfwSetWindowShouldClose(window, close);
	}

	void WindowHandler::setHandleWindowSizeCallback(bool allow)
	{
		handleWindowSizeCallback = allow;
	}

	bool WindowHandler::getHandleWindowSizeCallback() const
	{
		return handleWindowSizeCallback;
	}

	void WindowHandler::setWindowHeight(GLuint wHeight)
	{
		if (wHeight == 0){
			return;
		}
		windowHeight = wHeight;
	}

	GLuint WindowHandler::getWindowHeight() const
	{
		return windowHeight;
	}

	void WindowHandler::setWindowWidth(GLuint wWidth)
	{
		if (wWidth == 0){
			return;
		}
		windowWidth = wWidth;
	}

	GLuint WindowHandler::getWindowWidth() const
	{
		return windowWidth;
	}
}