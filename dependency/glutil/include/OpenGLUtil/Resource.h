#pragma once

namespace glutil{
	typedef unsigned int ResourceType;
	static const ResourceType R_VAO = 0;
	static const ResourceType R_VBO = 1;
	static const ResourceType R_EBO = 2;
	static const ResourceType R_TEX = 3;

	class ResourceBase{
	public:
		GLuint getResourceID() const { return resourceID; }
		virtual ~ResourceBase();
		inline static unsigned int getResourceCount(){ return resourceCount; }
	protected:
		GLuint resourceID;
		ResourceBase();
		ResourceBase(const ResourceBase&) = delete;
		ResourceBase& operator=(const ResourceBase&) = delete;
	private:
		static unsigned int resourceCount;
	};

	template<ResourceType R>
	class Resource : public ResourceBase{
	};

	typedef Resource<R_VAO> VAO;
	typedef Resource<R_VBO> VBO;
	typedef Resource<R_EBO> EBO;
	typedef Resource<R_TEX> Texture;

	template<>
	class Resource<R_VAO> : public ResourceBase{
	public:
		Resource();
		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;
		~Resource();
		inline static unsigned int getResourceCount(){ return resourceCount; }
		void bind() const;
	private:
		static unsigned int resourceCount;
	};

	template<>
	class Resource<R_VBO> : public ResourceBase{
	public:
		Resource();
		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;
		~Resource();
		inline static unsigned int getResourceCount(){ return resourceCount; }
		void bind() const;
	private:
		static unsigned int resourceCount;
	};

	template<>
	class Resource<R_EBO> : public ResourceBase{
	public:
		Resource();
		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;
		~Resource();
		inline static unsigned int getResourceCount(){ return resourceCount; }
	private:
		static unsigned int resourceCount;
	};

}

