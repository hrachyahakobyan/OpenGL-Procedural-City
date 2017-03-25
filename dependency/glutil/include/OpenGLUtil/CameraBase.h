#pragma once

namespace glutil{
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	/**
	*	@brief Base template class for Camera.
	*	Incorporates basic logic for FPS style movement.
	*	@author Hrachya Hakobyan
	*/

	class CameraBase
	{
	public:
		CameraBase();
		/**
		*	Main constructor.
		*	@param position the position of the viewer
		*	@param up the up vector in the world coordinates
		*	@param front the direction where the camera is looking at
		*/
		CameraBase(glm::vec3 position, glm::vec3 up, glm::vec3 front);
		CameraBase(const CameraBase&) = default;
		CameraBase& operator=(const CameraBase&) = default;
		virtual ~CameraBase() = default;

		/**
		*	Calculates the view matrix
		*	@return the view matrix
		*/
		virtual glm::mat4 view() const;

		/**
		*	Move the camera in the specified direction.
		*	@param direction the direction of movement
		*	@param deltaTime the step of the movement, the bigger is deltaTime, the larger will be the movement
		*/
		virtual void move(CameraMovement direction, GLfloat deltaTime);

		/**
		*	Rotates the camera. Default implementation does nothing.
		*	The meaning of parameters differs depending on implementation.
		*/
		virtual void rotate(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset = 0.0f);

		/**
		*	Zooms the camera.
		*	@param offset the offset of zoom. Could be the offset of mouse scroll.
		*/
		virtual void zoom(GLfloat offset);
		inline GLfloat getZoom() const;

		inline GLfloat getMovementSpeed() const;
		virtual void setMovementSpeed(GLfloat speed);

		inline GLfloat getMaxZoom() const;
		void setMaxZoom(GLfloat zoom);

		inline GLfloat getMinZoom() const;
		void setMinZoom(GLfloat minZoom);

	protected:
		// Camera Attributes
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		GLfloat movementSpeed;
		GLfloat maxZoom;
		GLfloat minZoom;
		GLfloat zoomLevel;
	private:
		/**
		*	Updates the camera attributes.
		*	Note the method is intentionally not virtual since it is called
		*	in the constructor.
		*/
		void update();
	};


	GLfloat CameraBase::getZoom() const
	{
		return zoomLevel;
	}

	GLfloat CameraBase::getMovementSpeed() const
	{
		return movementSpeed;
	}

	GLfloat CameraBase::getMaxZoom() const
	{
		return maxZoom;
	}

	GLfloat CameraBase::getMinZoom() const
	{
		return minZoom;
	}
}