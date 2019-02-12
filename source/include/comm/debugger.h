// @file: Debugger.h
// @biref: OpenGL 调试辅助类

#ifndef GL_DEBUGGER_H
#define GL_DEBUGGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


// Debugger 中文翻译连接： https://www.cnblogs.com/vertexshader/articles/3563883.html
// 英文 https://learnopengl.com/In-Practice/Debugging
class Debugger
{
public:

	static void setUpDebugInfo()
	{
		GLint flags; 
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
			glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

	}

	static GLenum glCheckError(const char *file, int line)
	{
		GLenum errorCode;
		while (errorCode = glGetError() != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
				case GL_INVALID_ENUM:					error = "INVALID_ENUM"; break;
				case GL_INVALID_VALUE:					error = "INVALID_VALUE"; break;
				case GL_INVALID_OPERATION:				error = "INVALID_OPERATION"; break;
				case GL_STACK_OVERFLOW:					error = "STACK_OVERFLOW"; break;
				case GL_STACK_UNDERFLOW:				error = "STACK_UNDERFLOW"; break;
				case GL_OUT_OF_MEMORY:					error = "OUT_OF_MEMORY"; break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}

	static void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam)
	{
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore error code

		std::cout << "_____________________________________________" << std::endl;
		std::cout << "Debug message (" << id << "):" << message << std::endl;
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:					std::cout << "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:			std::cout << "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:		std::cout << "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_APPLICATION:			std::cout << "Source: Application"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:			std::cout << "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_OTHER:					std::cout << "Source: Other"; break;
		}
		std::cout << std::endl;
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:					std::cout << "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		std::cout << "Type: Deprecated Behavior"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		std::cout << "Type: undefined behavior"; break;
			case GL_DEBUG_TYPE_PORTABILITY:				std::cout << "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:				std::cout << "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:					std::cout << "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:				std::cout << "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:				std::cout << "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:					std::cout << "Type: Other"; break;

		}
		std::cout << std::endl;

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;

		// todo  可以考虑在此处打印调用堆栈，以跟踪是哪一行代码导致的错误， 简单处理，则可以使用assert进行中断
		//assert(0);
		DebugBreak();

	}
};

#define glCheckError() Debugger::glCheckError(__FILE__, __LINE__)

#endif // GL_DEBUGGER_H