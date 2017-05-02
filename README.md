## Procedurally generated city with modern OpenGL and C++.
# ![City](media/pic2.png)

## Features
- Adjustable world dimensions. Can generate a world of arbitrary user defined size.
- Non-uniform terrain. The streets, blocks and alleys are generated using recursions and randomeness which gives a non-uniform look to the world.
![](media/pic3.png)
- Phong illumination model implemented in the fragment shader.
![](media/pic1.png)
- Realistic day-night transitioning using directional light.
![](media/pic10.png)
- Realistic fog using exponential squared formula and illumination dependent fog density.
![](media/pic7.png)
- Traffic
![](media/pic4.png)
- Collision detection between world objects and the viewer, i.e. the camera using spatial subdivision.

## Libraries and frameworks
- [OpenGLUtil](https://github.com/hrachyahakobyan/OpenGLUtil)
- [GLFW](http://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [GLM](http://glm.g-truc.net/0.9.8/index.html)
- [SOIL](http://www.lonesock.net/soil.html)
- [ASSIMP](http://assimp.sourceforge.net/)
