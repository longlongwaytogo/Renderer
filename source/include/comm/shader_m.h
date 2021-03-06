#ifndef SHADER_H
#define SHADER_H

#include <comm/glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
	Shader(){}
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
		loadFile(vertexPath, fragmentPath, geometryPath);
    }

	Shader(std::string& vertexPath, std::string&  fragmentPath)
	{
		loadFile(vertexPath.c_str(), fragmentPath.c_str());
	}

	void loadFile(const std::string& vertexPath, const std::string&  fragmentPath)
	{
		loadFile(vertexPath.c_str(), fragmentPath.c_str());
	}

	void loadFromSrc(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		const char* vShaderCode = vertexSrc.c_str();
		const char * fShaderCode = fragmentSrc.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX", "");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT", "");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM", "prgram");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void loadFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			if (geometryPath)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = geometryCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment, geometry;
		int success;
		char infoLog[512];
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX", vertexPath);
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT",fragmentPath);
		// geometry Shader
		if (geometryPath)
		{
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY", geometryPath);
		}
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryPath)
		{
			glAttachShader(ID, geometry);
		}
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM", "prgram");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
		unsigned int n = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(n, 1, GL_FALSE, &mat[0][0]);
		//glUniform4fv(n, 4, &mat[0][0]);
    }

	//////////////////////////////////////////////////////////////////////////
	// get
	void getBool(const std::string &name, bool& value) const
	{
		int ret = 0;
		glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &ret);
			value = ret == 0 ? false : true;
	}
	// ------------------------------------------------------------------------
	void getInt(const std::string &name, int& value) const
	{
		glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &value);
	}
	// ------------------------------------------------------------------------
	void getFloat(const std::string &name, float& value) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value);
	}
	// ------------------------------------------------------------------------
	void getVec2(const std::string &name, glm::vec2 &value) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value[0]);
	}
 
	// ------------------------------------------------------------------------
	void getVec3(const std::string &name, glm::vec3 &value) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value[0]);
	}
	
	// ------------------------------------------------------------------------
	void getVec4(const std::string &name, glm::vec4 &value) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value[0]);
	}

	// ------------------------------------------------------------------------
	void getMat2(const std::string &name, glm::mat2 &mat) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void getMat3(const std::string &name, glm::mat3 &mat) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void getMat4(const std::string &name, glm::mat4 &mat) const
	{
		glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &mat[0][0]);
	}

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type,std::string shaderFileName)
	{
		 
		GLint success;
		GLchar infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "[" + type + "]" << shaderFileName << std::endl;
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "[" + type + "]" << shaderFileName << std::endl; 
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

#endif