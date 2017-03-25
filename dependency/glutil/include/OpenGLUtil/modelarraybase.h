#pragma once
#include "Resource.h"
#include "Shader.h"
#define VERTEX_ATTRIB_VECTOR(vector) \
   		glEnableVertexAttribArray(initialAttrib); \
		glVertexAttribPointer(initialAttrib, vector::length(), ValueType<vector::value_type>::type, normalize, stride, (GLvoid*)offset); \
		glVertexAttribDivisor(initialAttrib, divisor); \
		finalAttrib = initialAttrib + 1;

#define VERTEX_ATTRIB_MATRIX(matrix) \
   				GLuint rowCount = matrix::col_type::length(); \
				GLuint colCount = matrix::row_type::length(); \
				GLsizei colSize = sizeof(matrix::col_type); \
				for (GLuint i = 0; i < colCount; i++){ \
					glEnableVertexAttribArray(initialAttrib + i); \
					glVertexAttribPointer(initialAttrib + i, rowCount, ValueType<matrix::value_type>::type, normalize, stride, (GLvoid*)(offset + i * colSize)); \
					glVertexAttribDivisor(initialAttrib + i, divisor); \
				} \
				finalAttrib = initialAttrib + colCount; 

namespace glutil{


	template<typename T>
	struct ValueType{
	};

	template<>
	struct ValueType<float>{ static const int type = GL_FLOAT; };

	template<>
	struct ValueType<int>{ static const int type = GL_INT; };

	template<>
	struct ValueType<unsigned int>{ static const int type = GL_UNSIGNED_INT; };

	template<>
	struct ValueType<double>{ static const int type = GL_DOUBLE; };

	template<typename T>
	struct VertexAttribPointer{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLint normalize, GLsizei offset, GLsizei stride = sizeof(T)){
			static_assert(false, "Vertex Attribute Pointers for type is not implemented");
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tvec3<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLint normalize, GLsizei offset, GLsizei stride = sizeof(T)){
			typedef glm::tvec3<T, glm::packed_highp> vector;
			VERTEX_ATTRIB_VECTOR(vector);
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tvec4<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLint normalize, GLsizei offset, GLsizei stride = sizeof(T)){
			typedef glm::tvec4<T, glm::packed_highp> vector;
			VERTEX_ATTRIB_VECTOR(vector);
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tmat4x4<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLint normalize, GLsizei offset, GLsizei stride = sizeof(T)){
			typedef glm::tmat4x4<T, glm::packed_highp> matrix;
			VERTEX_ATTRIB_MATRIX(matrix);
		}
	};

	template<std::size_t I, typename Tuple>
	struct AttribHelper
	{
		static void attrib(GLuint initialAttrib, GLuint& finalAttrib,
			GLuint divisor, GLint normalize,
			GLsizei offset, const GLsizei stride){
			AttribHelper<I - 1, Tuple>::attrib(initialAttrib, finalAttrib, divisor, normalize, offset, stride);
			GLsizei newOffset = offset + sizeof(detail::tuple_element<I-1, Tuple>::type);
			VertexAttribPointer<detail::tuple_element<I, Tuple>::type>()(finalAttrib, finalAttrib,
				divisor, normalize,
				newOffset, stride);
		}
	};

	template<typename Tuple>
	struct AttribHelper<0, Tuple>
	{
		static void attrib(GLuint initialAttrib, GLuint& finalAttrib,
			GLuint divisor, GLint normalize,
			GLsizei offset, const GLsizei stride){
			VertexAttribPointer<detail::tuple_element<0, Tuple>::type>()(initialAttrib, finalAttrib,
				divisor, normalize,
				offset, stride);
		}
	};

	template<typename M>
	class ModelArrayBase
	{
	public:
		ModelArrayBase() = delete;
		ModelArrayBase(M& model);
		virtual void draw(const Shader&) const = 0;
	protected:
		M model;
	};

	template<typename M>
	ModelArrayBase<M>::ModelArrayBase(M& m) :
		model(std::move(m))
	{
	}
}