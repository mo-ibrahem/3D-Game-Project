#pragma once
#pragma once

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"
#include"OBJLoader.h"

class Model
{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	

	void updateUniforms()
	{

	}

public:
	glm::vec3 position;
	Model(
		glm::vec3 position,
		Material* material,
		Texture* orTexDif,
		Texture* orTexSpec,
		std::vector<Mesh*>& meshes
	)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	//OBJ file loaded models
	Model(
		glm::vec3 position,
		Material* material,
		Texture* orTexDif,
		Texture* orTexSpec,
		const char* objFile
	)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		std::vector<Vertex> mesh = loadOBJ(objFile);
		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	//Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}
	void move(const glm::vec3 movement)
	{
		for (auto& i : this->meshes)
			i->move(movement);
	}
	void scaleUp(const glm::vec3 scale)
	{
		for (auto& i : this->meshes)
			i->scaleUp(scale);
	}
	void updateModelMatrix(const glm::mat4 newmat)
	{
		for (auto& i : this->meshes)
			i->updateModelMatrix(newmat);
	}

	void update()
	{

	}

	void render(Shader* shader)
	{
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Use a program (BECAUSE SHADER CLASS LAST UNIFORM UPDATE UNUSES IT)
		shader->use();

		//Draw
		for (auto& i : this->meshes)
		{
			//Activate texture for each mesh
			this->overrideTextureDiffuse->bind(0);
			this->overrideTextureSpecular->bind(1);

			i->render(shader); //Activates shader also
		}
	}
};