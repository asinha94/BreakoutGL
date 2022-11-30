# BreakoutGL (BrickBreaker in OpenGL)

Here's my implementation of the classic BrickBreaker developed with OpenGL and GLFW. Major kudos to [LearnOpenGL](https://learnopengl.com/) which walks though all the basics of graphics rendering and basic gamedev techniques.

![](https://github.com/asinha94/BreakoutGL/blob/main/breakout.gif)

Apologies for the choppy gif, I didn't look over any of the encoding options before creating the gif. Looks a lot better in person I swear!

# TODO	
- shader class doesn't detect file not found
- GLM and other libraries (glfw, assimp) should be added as git-submodules
- Migrate to CMakelists? VS fights with it every step of the way but would make it easier on macOS


# In case I need to recreate Visual Studio Project

- configuration properties
	- VC++ directories
		- include directores (my own headers)
		- library directories (lib headers)

- Linker -> 
	- Input
		- Additional Dependencies
			- glfw3.lib
			- opengl32.lib
			