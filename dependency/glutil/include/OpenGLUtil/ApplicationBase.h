#pragma once
#include "WindowHandler.h"

namespace glutil{

	class CameraBase;
	class WindowHandler;

	/**
	*	@brief Template class for Application.
	*	@author Hrachya Hakobyan
	*/
	class ApplicationBase : public CallbackListener
	{
	public:

		/**
		*	Convenience constructor. The application will create a default
		*	window with the given name.
		*	@param windowName the name of the window
		*/
		explicit ApplicationBase(const std::string& windowName);
		/**
		*	Application requires a window handler.
		*	The application will become the sole owner of the resource.
		*	@param windowHandler a unique pointer to a WindowHandler object
		*/
		explicit ApplicationBase(std::unique_ptr<WindowHandler> windowHandler);
		ApplicationBase() = delete;
		ApplicationBase(const ApplicationBase&) = delete;
		ApplicationBase& operator=(const ApplicationBase& other) = delete;
		virtual ~ApplicationBase();

		inline const WindowHandler& getWindowHandler() const;

		/**
		*	Sets the camera of the application.
		*	By default, the application does not have a camera.
		*	@param camera a unique_pointer to a CameraBase object
		*/
		void setCamera(std::unique_ptr<CameraBase> camera);

		/**
		*	Setup glue, glfw, window and other settings.
		*	Subclasses can override this method to add extra initialization steps.
		*	Cannot initialize twice.
		*	@throws std::exception if the initialization fails
		*/

		virtual void initialize();
		/**
		*	Starts the program loop.
		*/
		virtual void run();
		inline bool isInitialized() const;
	protected:
		bool initialized;
		std::unique_ptr<CameraBase> camera;
		std::unique_ptr<WindowHandler> windowHandler;
		bool keys[1024]; /** Keeps track of pressed keys*/
		bool isFirstMouse = true; /** Keeps track when the mouse first appears on the screen*/
		bool rightButtonPressed= false; /** Keeps track whether the right mouse button is pressed */
		bool allowZoom = false; /** Whether to allow zoom */
		GLfloat deltaTime = 0.0f; /** Keeps track of the duration of the previous frame*/
		GLfloat lastFrame = 0.0f; /** Keeps track of when the last frame finished.	*/
		GLfloat lastMouseX = 400; /** Keep track of mouse X coordinate*/
		GLfloat lastMouseY = 300; /** Keep track of mouse Y coordinate*/
		glm::vec4 backgroundColor; /** Sets the background color of the world*/

		GLint polygonMode = GL_FILL;
		/**
		*	Updates the render mode.
		*	By default, the keyboard button "T" will force to render triangles.
		*	Button "P" - points, button "L" - lines.
		*	Called during each game loop.
		*/
		virtual void updateRenderMode();

		/**
		*	Updates the delta frame. Called during each game loop.
		*/
		void updateDeltaFrame();

		/**
		*	This method is called at each iteration of the program loop after polling the user events 
		*	and before the call to render(). Override this method to update your view/mode/projection and model data
		*/
		virtual void updateData();

		/**
		*	This is the last method called before swapping buffers.
		*	Override this method to render your models.
		*/
		virtual void render();

		/**
		*	Called whenever a mouse button is pressed.
		*	Default implementation updates the values of members
		*	isFirstMouse and rightButtonIsPressed	
		*/
		virtual void mouseButtonCallback(GLFWwindow*, int, int, int);

		/**
		*	This method is called when mouse scroll is updated.
		*	By default, the application will attempto to change the zoom of the camera if
		*	the camera exists and zoom is allowed.
		*/
		virtual void mouseScrollCallback(GLFWwindow*, double, double);

		/**
		*	This method is called when the mouse position changes.
		*	By default, the application will attempt to rotate the camera if at meantime the right mouse button is pressed
		*	and if the camera exists.
		*	Override this method to change the logic.
		*/
		virtual void mousePositionCallback(GLFWwindow*, double, double);

		/**
		*	The method is called for each keyboard input.
		*	By default, this method updates the keys[] array.
		*/
		virtual void keyCallback(GLFWwindow*, int, int, int, int);
	};


	bool ApplicationBase::isInitialized() const
	{
		return initialized;
	}

	const WindowHandler& ApplicationBase::getWindowHandler() const
	{
		return *(windowHandler.get());
	}
}