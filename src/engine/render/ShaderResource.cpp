#include <render/ShaderResource.h>
#include <core/GlobalSettings.h>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <yaml-cpp/yaml.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace teliod::render
{
	//
	// Manager

	ShaderResourceManager::ShaderResourceManager()
	: core::ResourceManager<ShaderResource>()
	{
		// (will do conditional compilation)
		//
		// get all files in resources dir
		std::string pathString {core::GlobalSettings::getParam(core::GlobalSettings::RESOURCE_PATH) + "/shaders"};
		std::filesystem::path p{pathString};

		std::filesystem::directory_iterator it{p};

		for (auto & file : it)
		{
			if (file.is_directory())
				continue;

			std::string base_filename = file.path().string().substr(file.path().string().find_last_of("/\\") + 1);
			int lastDot = base_filename.find_last_of('.');
			std::string file_without_extension = base_filename.substr(0, lastDot);
			std::string file_extension = base_filename.substr(lastDot+1);

			if (file_extension != "yaml")
				continue;

			std::string vertexShaderFile;
			std::string fragmentShaderFile;

			YAML::Node descriptor = YAML::LoadFile(file.path().string());
			if (!descriptor.IsMap())
				continue;
			auto it2 = descriptor.begin();
			std::string shaderName {it2->second.as<std::string>()};
			++it2;
			auto stages = it2->second;

			for (auto it3=stages.begin(); it3!=stages.end(); ++it3)
			{
				std::string first{it3->first.as<std::string>()};
				std::string second{it3->second.as<std::string>()};
				if (first == "vs")
					vertexShaderFile = second;
				else if (first == "fs")
					fragmentShaderFile = second;
			}

			std::ifstream vsfile, fsfile;
			vsfile.open(pathString+"/"+vertexShaderFile);
			fsfile.open(pathString+"/"+fragmentShaderFile);
			std::stringstream vsstream, fsstream;
			vsstream << vsfile.rdbuf();
			fsstream << fsfile.rdbuf();
			vsfile.close();
			fsfile.close();

			std::string vs{vsstream.str()};
			std::string fs{fsstream.str()};
			const char * vs_cchar = vs.c_str();
			const char * fs_cchar = fs.c_str();

			GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vsID, 1, &vs_cchar, nullptr);
			glCompileShader(vsID);
			checkCompileErrors(vsID, "VERTEX");

			GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fsID, 1, &fs_cchar, nullptr);
			glCompileShader(fsID);
			checkCompileErrors(fsID, "FRAGMENT");

			GLuint programID = glCreateProgram();
			glAttachShader(programID, vsID);
			glAttachShader(programID, fsID);
			glLinkProgram(programID);
			checkCompileErrors(programID, "PROGRAM");

			glDeleteShader(vsID);
			glDeleteShader(fsID);

			// create resources
			ShaderResource * res = new ShaderResource();
			mResources.emplace(shaderName, res);
			res->shaderProgram = programID;
			res->type = shaderName;
		}
	}

	ShaderResource * ShaderResourceManager::createResource()
	{
		return nullptr;
	}

	ShaderResourceManager & ShaderResourceManager::getInstance()
	{
		static ShaderResourceManager instance;
		return instance;
	}

	void ShaderResourceManager::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}


	//
	// Resource

	ShaderResource::ShaderResource()
	: shaderProgram(0)
	{
	}

	ShaderResource::~ShaderResource()
	{
		glDeleteShader(shaderProgram);
	}

	unsigned int ShaderResource::getShaderProgram() const
	{
		return shaderProgram;
	}

	const std::string & ShaderResource::getType() const
	{
		return type;
	}
}
