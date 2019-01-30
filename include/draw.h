#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include	<iostream>
#include	<cstdlib>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* my_window_init();
unsigned int create_shaders(int a);
void load_texture(const char *path);	


