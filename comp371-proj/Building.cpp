#include "stdafx.h"
#include "Building.h"
#include "Random.h"
#include "Textures.h"

std::shared_ptr<glutil::Model> Building::make(BuildingType type, const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	switch (type){
	case Classical:
		return makeClassical(topleft, xWidth, zWidth, height);
	case Blocky:
		return makeBlocky(topleft, xWidth, zWidth, height);
	case Tower:
		return makeTower(topleft, xWidth, zWidth, height);
	}
	return nullptr;
}

std::shared_ptr<glutil::Model> Building::makeClassical(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) return nullptr;
	using namespace glutil;
	int tier_frac = 2 + Random::random(0, 3);
	int narrow_interval = 1 + Random::random(0, 1);
	int foundation = 1 + Random::random(0, 1);
	int ledge_height = 1;
	int bottom = 0;
	float rectWidth = 1.0f;
	float rectHeight = 1.0f;
	int tier_height = 0;
	int height_left = height;
	int num_tiers = 0;
	glm::vec3 origin = topleft;
	std::vector<Mesh> meshes;
	meshes.reserve(2);
	std::vector<Vertex> verticesWindow;
	std::vector<GLuint> indicesWindow;
	std::vector<Vertex> verticesLedge;
	std::vector<GLuint> indicesLedge;

	// Construct the foundation
	if (foundation + 1 < height){
		Mesh::rectangle(origin, xWidth, zWidth, rectWidth, rectHeight, foundation, false, true, verticesLedge, indicesLedge);
		bottom += foundation;
		origin.y = topleft.y + bottom;
	}
	while (true){
		height_left = height - bottom;
		tier_height = std::max(height_left / tier_frac, 2);
		if (height_left < 7){
			tier_height = height_left;
		}
		Mesh::rectangle(origin, xWidth, zWidth, rectWidth, rectHeight, tier_height, false, false, verticesWindow, indicesWindow);
		num_tiers++;
		bottom += tier_height;
		origin.y = topleft.y + bottom;
		// Construct the closing ledge
		if (bottom + ledge_height > height){
			break;
		}
		Mesh::rectangle(origin, xWidth, zWidth, rectWidth, rectHeight, ledge_height, false, true, verticesLedge, indicesLedge);
		bottom += ledge_height;
		origin.y = topleft.y + bottom;
		if (bottom > height){
			break;
		}
		if ((num_tiers % narrow_interval) == 0){
			if (xWidth > 3){
				origin.x += rectWidth;
				xWidth -= 2;
			}
			if (zWidth > 3){
				origin.z += rectWidth;
				zWidth -= 2;
			}
		}
	}
	meshes.push_back(Mesh(std::move(verticesWindow), std::move(indicesWindow)));
	meshes.push_back(Mesh(std::move(verticesLedge), std::move(indicesLedge)));
	meshes[0].addTexture(Textures::randomWindow());
	meshes[1].addTexture(Textures::randomFacade());
	auto model = std::shared_ptr<Model>(new Model(std::move(meshes)));
	constructRoof(*model.get(), Classical, origin, bottom, xWidth, zWidth);
	return model;
}

std::shared_ptr<glutil::Model> Building::makeBlocky(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) return nullptr;
	using namespace glutil;
	int left, right, front, back;
	int max_left, max_right, max_front, max_back;
	int mid_x, mid_z;
	int tiers, max_tiers;
	bool skip;
	int min_height;
	int foundation = 2;

	mid_x = topleft.x + xWidth / 2;
	mid_z = topleft.z + zWidth / 2;
	max_left = max_right = max_front = max_back = 1;
	min_height = 3;
	int half_xWidth = xWidth / 2;
	int half_zWidth = zWidth / 2;
	tiers = 0;
	max_tiers = 1 + height / 5 + height / 10 + height / 20;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Vertex> roofVertices;
	std::vector<GLuint> roofIndices;

	Mesh::rectangle(topleft, xWidth, zWidth, 1.0f, 1.0f, foundation, false, false, vertices, indices);
	Mesh::grid2D(glm::vec3{ topleft.x, foundation, topleft.z + zWidth}, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, xWidth, zWidth, roofVertices, roofIndices);
	height -= foundation;

	int roofHeight = height;
	glm::vec3 roofTopleft = topleft;
	int roofxWidth = xWidth;
	int roofzWidth = zWidth;
	
	while (true){
		if (height < min_height)
			break;
		if (tiers >= max_tiers)
			break;
		left = Random::random(1, half_xWidth);
		right = Random::random(1, half_xWidth);
		front = Random::random(1, half_zWidth);
		back = Random::random(1, half_zWidth);
		skip = false;
		if (left <= max_left && right <= max_right && front <= max_front && back <= max_back)
			skip = true;
		if (left == max_left || right == max_right || front == max_front || back == max_back)
			skip = true;
		if (!skip){
			max_left = std::max(max_left, left);
			max_right = std::max(right, max_right);
			max_front = std::max(front, max_front);
			max_back = std::max(back, max_back);
			glm::vec3 topleft{ mid_x - left, 0, mid_z - front };
			Mesh::rectangle(topleft, right + left, front + back, 1.0f, 1.0f, height, false, false, vertices, indices);
			if (tiers != 0)
				Mesh::grid2D(glm::vec3{ topleft.x, float(height), topleft.z + (front + back) }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, left + right, front + back, roofVertices, roofIndices);
			else
			// Save the roof coordinates
			{
				roofHeight = height;
				roofTopleft = glm::vec3{ topleft.x, float(height), topleft.z };
				roofxWidth = left + right;
				roofzWidth = front + back;
			}
			height -= Random::random(1, 5);
			tiers++;
		}
		height--;
	}
	std::vector<Mesh> meshes;
	meshes.reserve(3);
	meshes.push_back(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::randomWindow()}));
	meshes.push_back(Mesh(std::move(roofVertices), std::move(roofIndices), std::vector<std::shared_ptr<Texture>>{Textures::randomFacade()}));
	std::shared_ptr<Model> model(new Model(std::move(meshes)));
	constructRoof(*model.get(), Blocky, roofTopleft, roofHeight, roofxWidth, roofzWidth);
	return model;
}

std::shared_ptr<glutil::Model> Building::makeTower(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) return nullptr;
	using namespace glutil;
	int spans = 3 + Random::random(0, 8);
	float narrowScale = 0.2f;
	int max_tiers = 1 + height / 5 + height / 10 + height / 20;
	int tiers = 0;
	int tier_frac = 2 + Random::random(0, 3);
	int height_left = height;
	int tier_height = 0;
	float minEllipseArea = glm::pi<float>() * 2.0;
	bool isSimmetric = Random::random(0, 1);

	float xCenter = topleft.x + xWidth / 2.0;
	float zCenter = topleft.z + zWidth / 2.0;
	float step = glm::pi<float>() * 2.0 / spans;

	int xRadius = xWidth / 2.0;
	int zRadius = zWidth / 2.0;
	int bottom = 0;
	float angleStart = 0.0f;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Vertex> roofVertices;
	std::vector<GLuint> roofIndices;
	std::vector<Vertex> groundVertices;
	std::vector<GLuint> groundIndices;

	Mesh::grid2D(glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, xWidth, zWidth, groundVertices, groundIndices);
	while (true){
		if (tiers >= max_tiers)
			break;
		if (height_left <= 0)
			break;
		if (glm::pi<float>() * zRadius * xRadius < minEllipseArea)
			break;
		tier_height = std::max(height_left / tier_frac, 2);
		if (height_left < 7)
			tier_height = height_left;
		float angle = (isSimmetric ? 0.0f : angleStart);
		angleStart += step;
		std::vector<glm::vec3> pos(spans);
		for (std::size_t i = 0; i < spans; i++){
			float xCoord = xRadius * glm::cos(angle) + xCenter;
			float zCoord = zRadius * glm::sin(angle) + zCenter;
			pos[i] = glm::vec3{ xCoord, bottom, zCoord };
			angle = glm::mod((angle + step), 360.0f);
		}
		glm::vec3 d2{ 0.0f, 1.0f, 0.0f };
		for (std::size_t i = 0; i < spans; i++){
			glm::vec3 prev = pos[i];
			glm::vec3 next = pos[(i + 1) % spans];
			glm::vec3 bottomleft = prev;
			float distance = glm::distance(next, prev);
			glm::vec3 d1 = glm::normalize(next - prev);
			std::size_t windowCount = std::ceil(glm::distance(next, prev));
			d1 = d1 * (distance / windowCount);
			Mesh::grid2D(prev, d1, d2, glm::cross(d1, d2), windowCount, tier_height, vertices, indices);
		}
		height_left -= tier_height;
		bottom += tier_height;
		tiers++;
		xRadius = xRadius - xRadius * narrowScale;
		zRadius = zRadius - zRadius * narrowScale;
		triangulatePolygon(pos, roofVertices, roofIndices, bottom);
	}
	
	std::vector<Mesh> meshes;
	meshes.reserve(2);
	meshes.push_back(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::randomWindow()}));
	meshes.push_back(Mesh(std::move(roofVertices), std::move(roofIndices), std::vector<std::shared_ptr<Texture>>{Textures::randomGround()}));
	meshes.push_back(Mesh(std::move(groundVertices), std::move(groundIndices), std::vector<std::shared_ptr<Texture>>{Textures::randomGround()}));
	return std::shared_ptr<Model>(new Model(std::move(meshes)));
}



void Building::constructRoof(glutil::Model& building, BuildingType type, const glm::vec3& topleft, int height, int xWidth, int zWidth)
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Mesh::grid2D(glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, xWidth, zWidth, vertices, indices);
	building.add(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::randomGround()}));
}

// Assuming the polygon is in the counterclockwise direction
void Building::triangulatePolygon(const std::vector<glm::vec3>& points, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices, float height)
{
	if (points.size() < 3) return;
	using namespace glutil;
	std::size_t vertexSizeBefore = vertices.size();
	// calculate centroid
	glm::vec3 centroid;
	int xTex = 0;
	for (std::size_t i = 0; i < points.size(); i++){
		vertices.push_back(Vertex(glm::vec3{ points[i].x, height, points[i].z }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ float(xTex), 0.0f }));
		centroid += vertices.back().position;
		xTex = 1 - xTex;
	}
	centroid /= (points.size());
	vertices.push_back(Vertex(centroid, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ 0.5f, 1.0f }));
	int indexCounter = 0;
	GLuint last = points.size();
	for (std::size_t i = 0; i < points.size(); i++){
		indices.push_back(vertexSizeBefore + last);
		indices.push_back(vertexSizeBefore + i);
		indices.push_back(vertexSizeBefore + (i + 1) % points.size());
	}
}

void Building::constructSidewalk(const glm::vec3& topleft, int xWidth, int zWidth, int depth, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices)
{
	if (xWidth - 2 < depth || zWidth - 2 < depth)
		return;
	// Horizontal strip
	using namespace glutil;
	glm::vec3 bottomleft{ topleft.x, topleft.y, topleft.z + depth };
	glm::vec3 d1{ 1.0f, 0.0f, 0.0f };
	glm::vec3 d2{ 0.0f, 0.0f, -1.0f };
	glm::vec3 normal{ 0.0f, 1.0f, 0.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, xWidth, depth, vertices, indices);
	// Rightmost vertical strip
	bottomleft.x += (xWidth - depth);
	d1 = glm::vec3{ 0.0f, 0.0f, 1.0f };
	d2 = glm::vec3{ 1.0f, 0.0f, 0.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, zWidth - 2 * depth, depth, vertices, indices);
	// Bottom strip
	bottomleft = glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth };
	d1 = glm::vec3{ 1.0f, 0.0f, 0.0f };
	d2 = glm::vec3{ 0.0f, 0.0f, -1.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, xWidth, depth, vertices, indices);
	// Leftmost vertical strip
	bottomleft.z -= depth;
	bottomleft.x += depth;
	d1 = glm::vec3{ 0.0f, 0.0f, -1.0f };
	d2 = glm::vec3{ -1.0f, 0.0f, 0.0f };
	Mesh::grid2D(bottomleft, d1, d2, normal, zWidth - 2 * depth, depth, vertices, indices);
}