#pragma once

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <map>
#include <set>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

// SOIL

#include <SOIL.h>

#define NOEXCEPT _NOEXCEPT
#define FORWARD_MODELARRAY() \
		template<typename, typename...> \
		class ModelArray;

#define FORWARD_MODELARRAYBASE() \
		template<typename> \
		class ModelArrayBase;

#define FRIEND_MODELARRAY() \
		template<typename, typename...> \
		friend class ModelArray; \
		template<typename> \
		friend class ModelArrayBase;

#include "detail\standard_tuple.h"
#include "Camera.h"
#include "Shader.h"
#include "WindowHandler.h"
#include "ApplicationBase.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelArray.h"

 