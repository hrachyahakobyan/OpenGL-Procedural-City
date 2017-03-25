#pragma once
#include "Resource.h"

namespace glutil{
	/**
	*	@brief Texture resource.
	*	For optimization, textures cannot be constructed directly.
	*	Texture resources are created/accessed via static methods.
	*	from www.learnopengl.com
	*/

	enum MaterialType{
		Diffusive, Specular
	};

	template<>
	class Resource<R_TEX> : public ResourceBase {
	public:
		Resource(const Resource&) = delete;
		Resource& operator=(const Texture&) = delete;
		~Resource();
		void bind() const;

		const std::string path;
		const GLenum textureType;
		const MaterialType materialType;

		/**
		*	Loads a new texture from file.
		*	@param filename the filename of the texture image
		*	@param textureType the texture type
		*	@overwrite overwrite an existing texture in case of a duplication
		*	@return a shared pointer to a valid texture or nullptr if could not load the texture from the file
		*/
		static std::shared_ptr<Texture> fromFile(const std::string& filename, GLenum textureType, MaterialType mType);

		inline static std::size_t textureCount(){ return textures.size(); }

		/**
		*	Checks if a texture with a given path is already loaded
		*	@param path the path to the texture
		*/
		static bool isLoaded(const std::string& path);

		/**
		*	Get the texture with the given path.
		*	@param path the path of the texture
		*	@param texture a reference to a texture object that will be filled with the texture data if one is found
		*	@return true if the texture was found, false otherwise
		*/
		static std::shared_ptr<Texture> get(const std::string& path);

		static void purge();
	private:
		static unsigned int resourceCount;
		Resource(const std::string& path, GLenum textureType, MaterialType matType);
		static std::map<std::string, std::shared_ptr<Texture>> textures;
	};
}

