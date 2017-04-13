#include "stdafx.h"
#include "Building.h"
#include "Random.h"
#include "Textures.h"
#include "World.h"


Building::Building(const World& world, const std::vector<Area>& areas, std::size_t partitions) :
world(world), buildingGrids(partitions, std::vector<ModelPtr>(partitions))
{
	using namespace glutil;

	std::vector<std::vector<VertexIndexMap>> vertexIndexMapGrid(partitions, std::vector<VertexIndexMap>(partitions));
	int max = 100;
	for (auto& area : areas){
		auto coord = world.areaCoordinate(area, partitions, partitions);
		auto& currentMap = vertexIndexMapGrid[coord.second][coord.first];
		// Do stuff with the current map...
		fillArea(area, currentMap);
		max--;
		if (max == 0) break;
	}

	for (std::size_t row = 0; row < vertexIndexMapGrid.size(); row++){
		for (std::size_t col = 0; col < vertexIndexMapGrid[row].size(); col++){
			auto& currentMap = vertexIndexMapGrid[row][col];
			if (currentMap.empty()) continue;
			std::vector<Mesh> meshes;
			for (auto& VI : currentMap){
				auto& tuple = VI.second;
				meshes.push_back(Mesh(std::move(std::get<0>(tuple)), std::move(std::get<1>(tuple)), std::vector<std::shared_ptr<Texture>>{std::get<2>(tuple)}));
			}
			buildingGrids[row][col].reset(new glutil::Model(std::move(meshes)));
		}
	}
}

void Building::fillArea(const Area& area, VertexIndexMap& map)
{
	int height = calculateHeightForArea(area);
	makeBuilding(area, height, map);
}

void Building::makeBuilding(const Area& area, int height, VertexIndexMap& map)
{
	BuildingType type = static_cast<BuildingType>(Random::random(0, 3));
	switch (type){
	case Classical:
		 makeClassical(area, height, map);
		 break;
	case Blocky:
		makeBlocky(area, height, map);
		break;
	case Tower:
		makeTower(area, height, map);
		break;
	default:
		makeCube(area, height, map);
		break;
	}
}

void Building::update()
{
}

void Building::draw()
{
	for (const auto& row : buildingGrids){
		for (const auto& col : row){
			if (col)
				col->draw(world.getShader());
		}
	}
}

void Building::makeCube(const Area& area, int height, VertexIndexMap& map)
{
	if (area.getXWidth() < 2 || area.getZWidth() < 2)
		return;
	using namespace glutil;
	auto windowTexture = Textures::randomWindow();
	auto windowTextureID = windowTexture->getResourceID();
	std::get<2>(map[windowTextureID]) = windowTexture;
	std::vector<Vertex>& vertices = std::get<0>(map[windowTextureID]);
	std::vector<GLuint>& indices = std::get<1>(map[windowTextureID]);
	Mesh::rectangle(area.getTopleft(), area.getXWidth(), area.getZWidth(), 1.0f, 1.0f, height, false, false, vertices, indices);
	glm::vec3 roofAreaTopleft = area.getTopleft();
	roofAreaTopleft.y += height;
	makeRoof(Area::fromTopleft(roofAreaTopleft, area.getXWidth(), area.getZWidth()), Cube, map);
}

void Building::makeRoof(const Area& area, BuildingType type, VertexIndexMap& map)
{
	using namespace glutil;
	auto roofTexture = Textures::randomRoof();
	auto roofTextureID = roofTexture->getResourceID();
	auto& mapItemRef = map[roofTextureID];
	std::get<2>(mapItemRef) = roofTexture;
	std::vector<Vertex>& vertices = std::get<0>(mapItemRef);
	std::vector<GLuint>& indices = std::get<1>(mapItemRef);
	glm::vec3 bottomLeft = area.getBottomleft();
	Mesh::grid2D(bottomLeft, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, area.getXWidth(), area.getZWidth(), vertices, indices);
}

void Building::makeBlocky(const Area& area, int height, VertexIndexMap& map)
{
	int xWidth = area.getXWidth();
	int zWidth = area.getZWidth();
	if (xWidth < 2 || zWidth < 2)
		return;
	using namespace glutil;
	int left, right, front, back;
	int max_left, max_right, max_front, max_back;
	int mid_x, mid_z;
	int tiers, max_tiers;
	bool skip;
	int min_height;
	int foundation = 2;
	glm::vec3 topleft = area.getTopleft();

	mid_x = topleft.x + xWidth / 2;
	mid_z = topleft.z + zWidth / 2;
	max_left = max_right = max_front = max_back = 1;
	min_height = 3;
	int half_xWidth = xWidth / 2;
	int half_zWidth = zWidth / 2;
	tiers = 0;
	max_tiers = 1 + height / 5 + height / 10 + height / 20;

	auto windowTexture = Textures::randomWindow();
	auto windowTextureID = windowTexture->getResourceID();
	auto facadeTexture = Textures::randomFacade();
	auto facadeTextureID = facadeTexture->getResourceID();

	auto& windowItemRef = map[windowTextureID];
	auto& facadeItemref = map[facadeTextureID];
	std::get<2>(windowItemRef) = windowTexture;
	std::get<2>(facadeItemref) = facadeTexture;

	std::vector<Vertex>& vertices = std::get<0>(windowItemRef);
	std::vector<GLuint>& indices = std::get<1>(windowItemRef);
	std::vector<Vertex>& roofVertices = std::get<0>(facadeItemref);
	std::vector<GLuint>& roofIndices = std::get<1>(facadeItemref);

	Mesh::rectangle(topleft, xWidth, zWidth, 1.0f, 1.0f, foundation, false, false, vertices, indices);
	Mesh::grid2D(glm::vec3{ topleft.x, foundation, topleft.z + zWidth }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, xWidth, zWidth, roofVertices, roofIndices);
	height -= foundation;

	int roofHeight = height;
	Area roofArea;

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
				float roofXWidth = left + right;
				float rootfZWidth = front + back;
				roofArea = Area(glm::vec3{ topleft.x, roofHeight, topleft.z + rootfZWidth }, roofXWidth, rootfZWidth);
			}
			height -= Random::random(1, 5);
			tiers++;
		}
		height--;
	}
	makeRoof(roofArea, Blocky, map);
}

void Building::makeClassical(const Area& area, int height, VertexIndexMap& map)
{
	int xWidth = area.getXWidth();
	int zWidth = area.getZWidth();
	if (xWidth < 2 || zWidth < 2)
		return;
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
	glm::vec3 origin = area.getTopleft();
	glm::vec3 topleft = origin;
	
	auto windowTexture = Textures::randomWindow();
	auto windowTextureID = windowTexture->getResourceID();
	auto ledgeTexture = Textures::randomFacade();
	auto ledgeTextureID = ledgeTexture->getResourceID();

	auto& windowItemRef = map[windowTextureID];
	auto& ledgeItemRef = map[ledgeTextureID];
	std::get<2>(windowItemRef) = windowTexture;
	std::get<2>(ledgeItemRef) = ledgeTexture;

	std::vector<Vertex>& verticesWindow = std::get<0>(windowItemRef);
	std::vector<GLuint>& indicesWindow = std::get<1>(windowItemRef);
	std::vector<Vertex>& verticesLedge = std::get<0>(ledgeItemRef);
	std::vector<GLuint>& indicesLedge = std::get<1>(ledgeItemRef);

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
	makeRoof(Area::fromTopleft(origin, xWidth, zWidth), Classical, map);
}

void Building::makeTower(const Area& area, int height, VertexIndexMap& map)
{
	int xWidth = area.getXWidth();
	int zWidth = area.getZWidth();
	if (xWidth < 2 || zWidth < 2)
		return;
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
	glm::vec3 topleft = area.getTopleft();
	float xCenter = topleft.x + xWidth / 2.0;
	float zCenter = topleft.z + zWidth / 2.0;
	float step = glm::pi<float>() * 2.0 / spans;

	int xRadius = xWidth / 2.0;
	int zRadius = zWidth / 2.0;
	int bottom = 0;
	float angleStart = 0.0f;

	auto windowTexture = Textures::randomWindow();
	auto windowTextureID = windowTexture->getResourceID();
	auto facadeTexture = Textures::randomFacade();
	auto facadeTextureID = facadeTexture->getResourceID();

	auto& windowItemRef = map[windowTextureID];
	auto& facadeItemref = map[facadeTextureID];
	std::get<2>(windowItemRef) = windowTexture;
	std::get<2>(facadeItemref) = facadeTexture;

	std::vector<Vertex>& vertices = std::get<0>(windowItemRef);
	std::vector<GLuint>& indices = std::get<1>(windowItemRef);
	std::vector<Vertex>& roofVertices = std::get<0>(facadeItemref);
	std::vector<GLuint>& roofIndices = std::get<1>(facadeItemref);

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
			Mesh::grid2D(prev, d1, d2, glm::cross(d2, d1), windowCount, tier_height, vertices, indices);
		}
		height_left -= tier_height;
		bottom += tier_height;
		tiers++;
		xRadius = xRadius - xRadius * narrowScale;
		zRadius = zRadius - zRadius * narrowScale;
		triangulatePolygon(pos, roofVertices, roofIndices, bottom);
	}
}

std::shared_ptr<glutil::Model> Building::make(BuildingType type, const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	switch (type){
	case Classical:
		return makeClassical(topleft, xWidth, zWidth, height);
	case Blocky:
		return makeBlocky(topleft, xWidth, zWidth, height);
	case Tower:
		return makeTower(topleft, xWidth, zWidth, height);
	case Cube:
		return makeCube(topleft, xWidth, zWidth, height);
	}
	return nullptr;
}

std::shared_ptr<glutil::Model> Building::makeClassical(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) 
		return nullptr;
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
	constructRoof(*model.get(), Classical, origin, xWidth, zWidth);
	return model;
}

std::shared_ptr<glutil::Model> Building::makeBlocky(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) 
		return nullptr;
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
	constructRoof(*model.get(), Blocky, roofTopleft, roofxWidth, roofzWidth);
	return model;
}

std::shared_ptr<glutil::Model> Building::makeTower(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) 
		return nullptr;
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
			Mesh::grid2D(prev, d1, d2, glm::cross(d2, d1), windowCount, tier_height, vertices, indices);
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
	return std::shared_ptr<Model>(new Model(std::move(meshes)));
}

std::shared_ptr<glutil::Model> Building::makeCube(const glm::vec3& topleft, int xWidth, int zWidth, int height)
{
	if (xWidth < 2 || zWidth < 2) 
		return nullptr;
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Mesh::rectangle(topleft, xWidth, zWidth, 1.0f, 1.0f, height, false, false, vertices, indices);
	std::vector<Mesh> meshes;
	meshes.push_back(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::randomWindow()}));
	std::shared_ptr<Model> model(new Model(std::move(meshes)));
	constructRoof(*model.get(), Cube, glm::vec3{topleft.x, topleft.y + height, topleft.z}, xWidth, zWidth);
	return model;
}

void Building::constructRoof(glutil::Model& building, BuildingType type, const glm::vec3& topleft, int xWidth, int zWidth)
{
	using namespace glutil;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Mesh::grid2D(glm::vec3{ topleft.x, topleft.y, topleft.z + zWidth }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, xWidth, zWidth, vertices, indices);
	building.add(Mesh(std::move(vertices), std::move(indices), std::vector<std::shared_ptr<Texture>>{Textures::randomRoof()}));
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
		indices.push_back(vertexSizeBefore + (i + 1) % points.size());
		indices.push_back(vertexSizeBefore + i);
		indices.push_back(vertexSizeBefore + last);
	}
}

int Building::calculateHeightForArea(const Area& area) const
{
	float dist = glm::distance(area.getBottomleft(), world.center());
	int noise = std::max(0, 2 * Random::normal(0, 3));
	return std::max(2, int(maxHeight * calculateHeightCoefficientForDistance(dist)) + noise);
}

float Building::calculateHeightCoefficientForDistance(float dist) const
{
	float distMax = glm::distance(glm::vec3{ 0.0f, 0.0f, 0.0f }, world.center());
	return glm::max(glm::exp(-dist / distMax) - 1.0f / glm::e<float>(), 0.0f);
}


