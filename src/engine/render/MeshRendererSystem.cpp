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

			// set uniforms
			GLuint model = glGetUniformLocation(shader.getShaderProgram(), "model");
			GLuint view = glGetUniformLocation(shader.getShaderProgram(), "view");
			GLuint proj = glGetUniformLocation(shader.getShaderProgram(), "proj");

			const core::Camera & cam = core::Camera::getInstance();
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
