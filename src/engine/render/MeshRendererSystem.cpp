#include <render/MeshRendererSystem.h>
#include <render/Components.h>
#include <SceneGraph/Transform.h>
#include <core/MeshResource.h>
#include <core/Components.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Camera.h"

namespace teliod::render
{
	void MeshRendererSystem::render()
	{
		ecs::World & w = ecs::World::getInstance();
		for (ecs::Entity e : getEntities())
		{
			const core::MeshResource & mesh = *w.getComponent<core::MeshComponent>(e).getMesh();
			MeshRenderer & renderer = w.getComponent<MeshRenderer>(e);
			const render::ShaderResource & shader = renderer.getShaderResource();
			sg::Transform & tf = w.getComponent<sg::Transform>(e);

			const core::Camera & cam = core::Camera::getInstance();

			glUseProgram(shader.getShaderProgram());

			// set uniforms
			GLuint model = glGetUniformLocation(shader.getShaderProgram(), "model");
			GLuint view = glGetUniformLocation(shader.getShaderProgram(), "view");
			GLuint proj = glGetUniformLocation(shader.getShaderProgram(), "proj");

			if (renderer.textureResource != nullptr)
				glBindTexture(GL_TEXTURE_2D, renderer.textureResource->getMTexture());

			if (renderer.shaderResource->getType() == "phong_textured")
			{
				GLuint lightDirID = glGetUniformLocation(shader.getShaderProgram(), "lightDir");
				GLuint lightAmbientID = glGetUniformLocation(shader.getShaderProgram(), "lightAmbient");
				GLuint lightDiffuseID = glGetUniformLocation(shader.getShaderProgram(), "lightDiffuse");
				GLuint lightSpecularID = glGetUniformLocation(shader.getShaderProgram(), "lightSpecular");

				GLuint matSpecularID = glGetUniformLocation(shader.getShaderProgram(), "matSpecular");
				GLuint matShininessID = glGetUniformLocation(shader.getShaderProgram(), "matShininess");

				GLuint viewPosID = glGetUniformLocation(shader.getShaderProgram(), "viewPos");

				glUniform3f(lightDirID, 0.2f, 1.0f, 0.3f);
				glUniform3f(lightAmbientID, 0.2f, 0.2f, 0.2f);
				glUniform3f(lightDiffuseID, 0.4f, 0.4f, 0.4f);
				glUniform3f(lightSpecularID, 0.5f, 0.5f, 0.5f);

				glUniform3f(matSpecularID, 0.5f, 0.5f, 0.5f);
				glUniform1f(matShininessID, 32.0f);

				glUniform3fv(viewPosID, 1, &cam.getTransform().getWorldTransform()[3][0]);
			}

			glUniformMatrix4fv(model, 1, GL_FALSE, &tf.getWorldTransform()[0][0]);
			glUniformMatrix4fv(view, 1, GL_FALSE, &cam.getViewMatrix()[0][0]);
			glUniformMatrix4fv(proj, 1, GL_FALSE, &cam.getProjMatrix()[0][0]);
			// bind vao
			glBindVertexArray(renderer.getVAO());
			// drawElements()
			glDrawElements(GL_TRIANGLES, mesh.getNumTriangles()*3, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	}
}
