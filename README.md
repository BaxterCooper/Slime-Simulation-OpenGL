# Slime-Simulation-OpenGL
 
This simulation was inspired by Sebastian Lague's YouTube video [Coding Adventure: Ant and Slime Simulations](https://www.youtube.com/watch?v=X-iSQQgOd1A) which itself was based on a research paper by Jones, Jeff found here https://uwe-repository.worktribe.com/output/980579.

For this project, I didn't want to be pulled into using the Unity game engine and so this project is written in C++ using the OpenGL graphics library.

## Requirements

This project makes use of compute shaders which were added to OpenGL in version 4.3. Unfortunately macOS depricated support for OpenGL at version 4.1 and so this project cannot be run on macOS.

- `OpenGL 4.6`
- `GLFW 3.3.8`
- `Glad 4.6`
- `GLM`

## Installation

### GLFW

The instructions for installing GLFW can be found on their [website](https://www.glfw.org).

The GLFW header files are already contained in the `./include` directory.

GLFW has pre-compiled binaries on their website, instructions for compiling these binaries from source code can also be found on their website. 

These binaries must be placed into the `./lib` directory to be used with the Makefile provided.

## Compilation

`make`

## Execution

`./main`







