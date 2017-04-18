#pragma once

struct Volume;

/**
*	@brief Represents a rectangular area in the XZ plane.
*	@author Hrachya Hakobyan
*/
struct Area{
	/**
	*	Subdivide the area into smaller subareas. The sum of the areas of subareas
	*	need not necessarily be equal to the original one. The subareas, based on probability,
	*	may have a space between each other. The subdivison can yield from 1 to 4 subareas, based on chance
	*	and the dimensions of the initial area.
	*	@param subAreas the vector to add the new areas
	*	@param minXWidth the minimum width in X direction of resulting subareas
	*	@param minZWidth the minimum width in negative Z direction of resulting subareas
	*/
	void subdivide(std::vector<Area>& subAreas, int minXWidth, int minZWidth) const;
	/**
	*	get the area 
	*	@return the product of width in X and negative Z directions
	*/
	inline int area() const { return xWidth * zWidth; }
	/**
	*	@return the topleft corner
	*/
	inline glm::vec3 getTopleft() const{ return glm::vec3{ bottomleft.x, bottomleft.y, bottomleft.z - zWidth }; }
	/**
	*	@return the bottomleft corner
	*/
	inline glm::vec3 getBottomleft() const { return bottomleft; }
	/**
	*	@return the center 
	*/
	inline glm::vec3 getCenter() const { return{ bottomleft.x + xWidth / 2, bottomleft.y, bottomleft.z - zWidth / 2 }; }
	/**
	*	@return the bottomright corner
	*/
	inline glm::vec3 getBottomRight() const { return glm::vec3{ bottomleft.x + xWidth, bottomleft.y, bottomleft.z }; }
	/**
	*	@return the topright corner
	*/
	inline glm::vec3 getTopRight() const {return glm::vec3{ bottomleft.x + xWidth, bottomleft.y, bottomleft.z - zWidth };}
	/**
	*	@return the width in X direction
	*/
	inline int getXWidth() const { return xWidth; }
	/**
	*	@return the width in negative Z direction
	*/
	inline int getZWidth() const { return zWidth; }
	/**
	*	Void set the bottomleft corner of the area
	*	@param bottomleft the new bottomleft corner of the area
	*/
	inline void setBottomleft(const glm::vec3& bottomleft) { this->bottomleft = bottomleft; }
	inline void setXWidth(int xWidth) { this->xWidth = xWidth; }
	inline void setZWidth(int zWidth) { this->zWidth = zWidth; }
	/**
	*	Add a new volume to this area
	*	@param vol a volume object to be added
	*/
	void addVolume(const Volume& vol);
	/**
	*	Check if any volume in this area contains the specified 3D point
	*	@true if any volume in this area contains the point, false otherwise
	*/
	bool contains(const glm::vec3& pt) const;
	Area(){}
	Area(const glm::vec3& bottomleft, int xWidth, int zWidth);
	~Area();
	/**
	*	Construct an area object from the topleft corner
	*	@return an area object 
	*/
	static Area fromTopleft(const glm::vec3& topleft, int xWidth, int zWidth){
		return Area(glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth }, xWidth, zWidth);
	}
private:
	void divideAlongX(std::vector<Area>& subareas, int minXWidth) const;
	void divideAlongZ(std::vector<Area>& subareas, int minZWidth) const;
	glm::vec3 bottomleft;
	std::vector<Volume> volumes;
	int xWidth;
	int zWidth;
};

/**
*	@brief Class representing a right rectangular prism
*	@author Hrachya Hakobyan
*/
struct Volume
{
public:
	/**
	*	@param area the are in XZ plane, the bottom side of the prism
	*	@param height the height of the prism
	*/
	Volume(const Area& area, float height);
	~Volume();

	/**
	*	@return the area in XZ plane
	*/
	inline const Area& getArea() const { return area; }
	inline float getHeight() const { return height; }
	/**
	*	Check if the volume contains the specified 3D point
	*	@param point the point to check against
	*	@return true if the point is contained inside the prism
	*/
	bool contains(const glm::vec3& point) const;
private:
	Area area;
	float height;
};
