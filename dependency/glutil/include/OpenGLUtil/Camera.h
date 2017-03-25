#pragma once
#include "CameraBase.h"

namespace glutil{
	/**
	*	@brief Camera with possibility to rotate.
	*	Uses Euler angles to process rotation.
	*	@author Hrachya Hakobyan
	*/
	class Camera : public CameraBase
	{
	public:
		Camera();
		Camera(glm::vec3 position,
			glm::vec3 up,
			glm::vec3 front,
			GLfloat yaw = -90.0f,
			GLfloat pitch = 0.0f,
			GLfloat roll = 0.0f);

		/**
		*	Rotates the camera using Euler angles.
		*	@param xoffset the magnitude of rotation in XZ plane, a.k.a yaw
		*	@param yoffset the magnitude of ration in ZY plane, a.k.a pitch
		*	@param zoffset the magnitude of ration in XY plane, a.k.a roll
		*/
		void rotate(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset = 0.0f);

		inline GLfloat getMaxPitch() const;

		/**
		*	@brief sets the max pitch value.
		*	With Euler angles, the pitch must be restricted to avoid
		*	Gimbal locks.
		*	@param maxPitch the maximum pitch in degrees
		*/
		void setMaxPitch(GLfloat maxPitch);
	private:
		GLfloat maxPitch; /**The maximum allowed pitch	*/
		GLfloat yaw; 
		GLfloat pitch;
		GLfloat roll;
		GLfloat rotationSensitivity;
		void update();
	};

	GLfloat Camera::getMaxPitch() const
	{
		return maxPitch;
	}
}