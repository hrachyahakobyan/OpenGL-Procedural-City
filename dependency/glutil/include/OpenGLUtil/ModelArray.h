#pragma once
#include "ModelArrayBase.h"

namespace glutil{

	template<typename M, typename... Types>
	class ModelArray : public ModelArrayBase<M>
	{
	public:
		typedef tuple<Types...> Tuple;
		typedef std::vector<Tuple> InstanceData;
		static const std::size_t TypeCount = sizeof...(Types);
		static const GLsizei DataSize = sizeof(Tuple);

		ModelArray() = delete;
		ModelArray(M& m);	
		ModelArray(M& m, const InstanceData& data);

		InstanceData instanceData;
		std::shared_ptr<VBO> vbo;
		GLuint initialAttrib;

		void bufferData();
		void draw(const Shader& shader) const;
	private:
	
	};

	template<typename M, typename... Types>
	ModelArray<M, Types...>::ModelArray(M& m):
		ModelArrayBase<M>(m),
		vbo(new VBO())
	{
	}

	template<typename M, typename... Types>
	ModelArray<M, Types...>::ModelArray(M& m, const InstanceData& iData) :
		ModelArrayBase<M>(m),
		instanceData(iData),
		vbo(new VBO())
	{
	}

	template<typename M, typename... Types>
	void ModelArray<M, Types...>::bufferData()
	{
		vbo->bind();
		glBufferData(GL_ARRAY_BUFFER, instanceData.size() * DataSize, instanceData.data(), GL_STATIC_DRAW);
		GLsizei offset = 0;
		GLuint initAttrib = initialAttrib;
		GLuint finalAttrib = 0;
		for (std::size_t i = 0; i < model.meshes.size(); i++){
			model.meshes[i].vao->bind();
			AttribHelper<TypeCount - 1, Tuple>::attrib(initAttrib, finalAttrib, 1, GL_FALSE, 0, DataSize);
			glBindVertexArray(0);
		}
	}

	template<typename M, typename... Types>
	void ModelArray<M, Types...>::draw(const Shader& shader) const
	{
		shader.use();
		for (GLuint i = 0; i < model.meshes.size(); i++)
		{
			model.meshes[i].bindTextures(shader);
			model.meshes[i].vao->bind();
			glDrawElementsInstanced(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, instanceData.size());
			glBindVertexArray(0);
			model.meshes[i].unbindTextures();
		}
	}
}


