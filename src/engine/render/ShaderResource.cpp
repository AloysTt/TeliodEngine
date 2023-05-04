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

    // Source for the function isIdentifier() below : https://www.geeksforgeeks.org/check-whether-the-given-string-is-a-valid-identifier/
    // Function that returns true if str is a valid identifier
    bool ShaderResourceManager::isIdentifier( std::string& str )
    {
        // If first character is invalid
        if (!((str[0] >= 'a' && str[0] <= 'z')
              || (str[0] >= 'A' && str[0] <= 'Z')
              || str[0] == '_'))
            return false;

        // Traverse the string for the rest of the characters
        for (int i = 1; i < str.length(); i++) {
            if (!((str[i] >= 'a' && str[i] <= 'z')
                  || (str[i] >= 'A' && str[i] <= 'Z')
                  || (str[i] >= '0' && str[i] <= '9')
                  || str[i] == '_'))
                return false;
        }

        // String is a valid identifier
        return true;
    }

    bool ShaderResourceManager::Add_Attr( std::vector<struct ShaderResource::Attr >& vec, std::string& Attr )
    {
        std::string parse_str;
        int word_idx = 0;
        bool is_uniform = false;
        std::string type_name;

        const std::vector< std::string > ValidTypes{ "vec2", "vec3", "vec4", "int", "float", "double", "mat3", "mat4", "sampler2D" };

        for( unsigned long i = 0; i < Attr.length(); i++ )
        {
            if( Attr[i] ==  ' ' )
            {
                switch( word_idx )
                {
                    case 0 : if( parse_str == "uniform" )
                                is_uniform = true;
                            else if( parse_str != "in" && parse_str != "out" ) {
                            std::cout << parse_str << " is not a valid prefix to an attribute !\n";
                            return false;
                        }
                        break;
                    case 1 : if( std::find( ValidTypes.begin(), ValidTypes.end(), parse_str ) == ValidTypes.end() ){
                            std::cout << parse_str << " is not a valid type for an attribute !\n";
                            return false;
                        }
                        type_name = parse_str;
                        break;
                    default :
                        std::cout << "There are too many words in the attribute " << Attr <<  " !\n";
                        return false;
                }

                parse_str.clear();
                word_idx++;
            }
            else
                parse_str.push_back(Attr[i]);
        }

        if( word_idx < 2 ){
            std::cout << "There are not enough words in the attribute " << Attr <<  " !\n";
            return false;
        }

        if( !isIdentifier( parse_str ) ){
            std::cout << parse_str << " is not a valid name for an attribute !\n";
            return false;
        }
        else if( is_uniform )// Only uniform attributes can be modified from the CPU
        {
            struct ShaderResource::Attr a;
            a.name = parse_str;
            a.type = type_name;
            a.data = nullptr;
            vec.push_back(a);
        }

        return true;
    }

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
            std::vector<struct ShaderResource::Attr > vertAttr;
			std::string fragmentShaderFile;
            std::vector<struct ShaderResource::Attr > fragAttr;

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
				//std::string second{};

				if (first == "vs")
					vertexShaderFile = it3->second.as<std::string>();
				else if (first == "fs")
					fragmentShaderFile = it3->second.as<std::string>();
                else if( first == "va" )
                {
                    auto Attrs = it3->second;

                    for( unsigned long i = 0; i < Attrs.size(); i++ )
                    {
                        std::string attr{Attrs[i].as<std::string>()};
                        if( !Add_Attr( vertAttr, attr ) )
                            std::cout << "In Shader Resource Manager : " << attr << " is not a valid attribute for a shader\n";
                    }
                }
                else if( first == "fa" )
                {
                    auto Attrs = it3->second;

                    for( unsigned long i = 0; i < Attrs.size(); i++ )
                    {
                        std::string attr{Attrs[i].as<std::string>()};
                        if( !Add_Attr( fragAttr, attr ) )
                            std::cout << "In Shader Resource Manager : " << attr << " is not a valid attribute for a shader\n";
                    }
                }
                else
                    std::cout << "In Shader Resource Manager : does not know " << first << " as first\n";
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
            res->vertAttr = vertAttr;
            res->fragAttr = fragAttr;
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

    void ShaderResource::setUniform( bool is_in_vs, std::string& name, void* data )
    {
        std::vector<struct ShaderResource::Attr >& vec = (is_in_vs)?(vertAttr):(fragAttr);

        for( unsigned int i = 0; i < vec.size(); i++ )
            if( vec[i].name == name )
            {
                vec[i].data = data;
                return;
            }
    }

    void ShaderResource::updateShader() const
    {
        //std::cout << "---" << shaderName << "\n";
        for( auto& Attr : vertAttr )
            std::cout << " va : " << Attr.name << " ( " << Attr.type << " ) -> " << (bool) Attr.data << "\n";
        for( auto& Attr : fragAttr )
            std::cout << " fa : " << Attr.name << " ( " << Attr.type << " ) -> " << (bool) Attr.data << "\n";

        for( auto& attr : vertAttr )
        {
            if( !attr.data )
                continue;

            GLint loc = glGetUniformLocation( shaderProgram, attr.name.c_str());

            if( attr.type == "float" ) glUniform1fv( loc, 1, (float*)attr.data);
            else if( attr.type == "vec2" ) glUniform2fv( loc, 1, (float*)attr.data);
            else if( attr.type == "vec3" ) glUniform3fv( loc, 1, (float*)attr.data);
            else if( attr.type == "vec4" ) glUniform4fv( loc, 1, (float*)attr.data);
            else if( attr.type == "int" ) glUniform1iv( loc, 1, (int*)attr.data);
            else if( attr.type == "mat3" ) glUniformMatrix3fv( loc, 1, GL_FALSE, (float*)attr.data);
            else if( attr.type == "mat4" ) glUniformMatrix4fv( loc, 1, GL_FALSE, (float*)attr.data);
            else if( attr.type == "sampler2D" );
        }

        for( auto& attr : fragAttr )
        {
            if( !attr.data )
                continue;

            GLint loc = glGetUniformLocation( shaderProgram, attr.name.c_str());

            if( attr.type == "float" ) glUniform1fv( loc, 1, (float*)attr.data);
            else if( attr.type == "vec2" ) glUniform2fv( loc, 1, (float*)attr.data);
            else if( attr.type == "vec3" ) glUniform3fv( loc, 1, (float*)attr.data);
            else if( attr.type == "vec4" ) glUniform4fv( loc, 1, (float*)attr.data);
            else if( attr.type == "int" ) glUniform1iv( loc, 1, (int*)attr.data);
            else if( attr.type == "mat3" ) glUniformMatrix3fv( loc, 1, GL_FALSE, (float*)attr.data);
            else if( attr.type == "mat4" ) glUniformMatrix4fv( loc, 1, GL_FALSE, (float*)attr.data);
            else if( attr.type == "sampler2D" );
        }
    }

	unsigned int ShaderResource::getShaderProgram() const
    {
		return shaderProgram;
	}
}
