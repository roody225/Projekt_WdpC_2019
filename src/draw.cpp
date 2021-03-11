#include"../include/draw.h"

using namespace std;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	TexCoord = aTexCoord;\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"	FragColor = texture(tex, TexCoord);\n"
"}\n";
const char *fragmentShaderSourcenotex = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = ourColor;\n"
"}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* my_window_init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(1200, 900, "Kamienne Więzienie v1.0", NULL, NULL);
	
	if(window == NULL)
	{
		cerr<<"Failed to create GLFW window\n";
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr<<"Failed to initialize GLAD\n";
		exit(1);
	}
	glViewport(0, 0, 1200, 900);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	return window;
}

unsigned int create_shaders(int a)
{
	unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success=1;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cerr<<"ERROR::VERTEX_SHADER::COMPILATION\n"<<infoLog<<"\n";
	}
	
	unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	if(a==0)
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	else
		glShaderSource(fragmentShader, 1, &fragmentShaderSourcenotex, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cerr<<"ERROR::FRAGMENT_SHADER::COMPILATION\n"<<infoLog<<"\n";
	}
	
	unsigned int shaderProgram=glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cerr<<"ERROR::SHADERS::LINKING\n"<<infoLog<<"\n";
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
void load_texture(const char *path)
{
	unsigned int texture;
	int texw, texh, nrCh;
	unsigned char *data;

	data=stbi_load( path , &texw , &texh , &nrCh , 0); 
	glGenTextures(1, &texture); 
	glBindTexture(GL_TEXTURE_2D, texture); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	glGenerateMipmap(GL_TEXTURE_2D); 
	stbi_image_free(data);
}

