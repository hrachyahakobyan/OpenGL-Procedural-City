#include "stdafx.h"
#include "Building.h"
#include "Random.h"
#include "Textures.h"
#include "World.h"


Building::Building(const World& world, std::vector<Area>& areas, std::size_t partitions) :
world(world), buildingGrids(partitions, std::vector<ModelPtr>(partitions))
{
	using namespace glutil;

	std::vector<std::vector<VertexIndexMap>> vertexIndexMapGrid(partitions, std::vector<VertexIndexMap>(partitions));
	int max = 500;
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

void Building::fillArea(Area& area, VertexIndexMap& map)
{
	int height = calculateHeightForArea(area);
	makeBuilding(area, height, map);
}

void Building::makeBuilding(Area& area, int height, VertexIndexMap& map)
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

void Building::makeCube(Area& area, int height, VertexIndexMap& map)
{
	if (area.getXWidth() < 2 || area.getZWidth() < 2)
		return;
	using namespace glutil;
	auto windowTexture = Textures::randomWindow();
	auto windowTextureID = windowTexture->getResourceID();
	std::get<2>(map[windowTextureID]) = windowTexture;
	std::vector<Vertex>& vertices = std::get<0>(map[windowTextureID]);
	std::vector<GLuint>& indices = std::get<1>(map[windowTextureID]);
	area.addVolume(Volume(area, height));
	Mesh::rectangle(area.getTopleft(), area.getXWidth(), area.getZWidth(), 1.0f, 1.0f, height, false, false, vertices, indices);
	glm::vec3 roofAreaTopleft = area.getTopleft();
	roofAreaTopleft.y += height;
	makeRoof(Area::fromTopleft(roofAreaTopleft, area.getXWidth(), area.getZWidth()), Cube, map, area);
}

void Building::makeRoof(const Area& area, BuildingType type, VertexIndexMap& map, Area& bldArea)
{
	using namespace glutil;
	
	glm::vec3 bottomLeft = area.getBottomleft();
	glm::vec3 topLeft = area.getTopleft();

	if (type == Tower){
		auto texture = Textures::randomRoof();
		auto textureID = texture->getResourceID();
		auto& mapItemRef = map[textureID];
		std::get<2>(mapItemRef) = texture;
		std::vector<Vertex>& vertices = std::get<0>(mapItemRef);
		std::vector<GLuint>& indices = std::get<1>(mapItemRef);
		Mesh::grid2D(bottomLeft, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, area.getXWidth(), area.getZWidth(), vertices, indices);
	}
	else {
		bool skip = Random::dice(0.25f);
		if (skip){
			auto texture = Textures::randomRoof();
			auto textureID = texture->getResourceID();
			auto& mapItemRef = map[textureID];
			std::get<2>(mapItemRef) = texture;
			std::vector<Vertex>& vertices = std::get<0>(mapItemRef);
			std::vector<GLuint>& indices = std::get<1>(mapItemRef);
			Mesh::grid2D(bottomLeft, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, area.getXWidth(), area.getZWidth(), vertices, indices);
			return;
		}
		bool makeRoof = Random::dice(0.5f);
		if (area.getXWidth() < 6 || area.getZWidth() < 6)
			makeRoof = true;
		glm::vec3 bottomRight = area.getBottomRight();
		glm::vec3 topRight = area.getTopRight();
		if (makeRoof){
			auto texture = (type == Classical ? Textures::randomDarkBrick() : Textures::randomRoofTile());
			auto textureID = texture->getResourceID();
			auto& mapItemRef = map[textureID];
			std::get<2>(mapItemRef) = texture;
			std::vector<Vertex>& vertices = std::get<0>(mapItemRef);
			std::vector<GLuint>& indices = std::get<1>(mapItemRef);
			std::vector<glm::vec3> points(4);
			points[3] = topLeft;
			points[2] = bottomLeft;
			points[1] = bottomRight;
			points[0] = topRight;
			double roofingHeight = 1.0f;
			if (type == Classical){
				roofingHeight = double(Random::random(2.0f, 4.0f));
			}
			bldArea.addVolume(Volume(area, roofingHeight));
			triangulatePolygon(points, vertices, indices, bottomLeft.y, roofingHeight);
		}
		else if (!makeRoof) {
			auto texture = Textures::randomLightBrick();
			auto textureID = texture->getResourceID();
			auto& mapItemRef = map[textureID];
			std::get<2>(mapItemRef) = texture;
			std::vector<Vertex>& vertices = std::get<0>(mapItemRef);
			std::vector<GLuint>& indices = std::get<1>(mapItemRef);
			int cubeXWidth = Random::random(3, area.getXWidth() / 2);
			int cubeZWidth = Random::random(3, area.getZWidth() / 2);
			int cubeTopleftX = Random::random(topLeft.x, topRight.x - cubeXWidth);
			int cubeTopleftZ = topLeft.z + Random::random(0, area.getZWidth() - cubeZWidth);
			int cubeHeight = Random::random(1, 2);
			glm::vec3 topleft{ cubeTopleftX, topLeft.y, cubeTopleftZ };
			Mesh::rectangle(topleft, cubeXWidth, cubeZWidth, 1.0f, 1.0f, cubeHeight, false, true, vertices, indices);
			bldArea.addVolume(Volume(Area::fromTopleft(topleft, cubeXWidth, cubeZWidth), cubeHeight));
			{
				auto texture = Textures::randomRoof();
				auto textureID = texture->getResourceID();
				auto& mapItemRef = map[textureID];
				std::get<2>(mapItemRef) = texture;
				std::vector<Vertex>& vertices = std::get<0>(mapItemRef);
				std::vector<GLuint>& indices = std::get<1>(mapItemRef);
				Mesh::grid2D(bottomLeft, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, area.getXWidth(), area.getZWidth(), vertices, indices);

			}
		}
	}
}

void Building::makeBlocky(Area& area, int height, VertexIndexMap& map)
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
	max_tiers = 1 + height / 4 + height / 8 + height / 16;

	auto windowTexture = Textures::randomWindow();
	auto windowTextureID = windowTexture->getResourceID();
	auto facadeTexture = Textures::randomRoof();
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
	area.addVolume(Volume(area, foundation));
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
			area.addVolume(Volume(Area::fromTopleft(topleft, right + left, front + back), height));
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
			height -= Random::random(1, 3);
			tiers++;
		}
		else {
			height--;
		}
	}
	makeRoof(roofArea, Blocky, map, area);
}

void Building::makeClassical(Area& area, int height, VertexIndexMap& map)
{
	int xWidth = area.getXWidth();
	int zWidth = area.getZWidth();
	if (xWidth < 2 || zWidth < 2)
		return;
	using namespace glutil;
	int tier_frac = 4 + Random::random(0, 3);
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
		area.addVolume(Volume(Area::fromTopleft(origin, xWidth, zWidth), foundation));
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
		area.addVolume(Volume(Area::fromTopleft(origin, xWidth, zWidth), tier_height));
		num_tiers++;
		bottom += tier_height;
		origin.y = topleft.y + bottom;
		// Construct the closing ledge
		if (bottom + ledge_height > height){
			break;
		}
		Mesh::rectangle(origin, xWidth, zWidth, rectWidth, rectHeight, ledge_height, false, true, verticesLedge, indicesLedge);
		area.addVolume(Volume(Area::fromTopleft(origin, xWidth, zWidth), ledge_height));
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
	makeRoof(Area::fromTopleft(origin, xWidth, zWidth), Classical, map, area);
}

void Building::makeTower(Area& area, int height, VertexIndexMap& map)
{
	int xWidth = area.getXWidth();
	int zWidth = area.getZWidth();
	if (xWidth < 2 || zWidth < 2)
		return;
	using namespace glutil;
	height += height / 4;
	int spans = 3 + Random::random(0, 8);
	float narrowScale = 0.2f;
	int max_tiers = 1 + height / 4 + height / 8 + height / 16;
	int tiers = 0;
	int tier_frac = 3 + Random::random(0, 2);
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
	auto facadeTexture = Textures::randomRoof();
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
		area.addVolume(Volume(Area(glm::vec3{ xCenter - xRadius, bottom, zCenter + zRadius}, 2 * xRadius, 2 * zRadius), tier_height));
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

	if (height_left != 0){
		int x = 100;
	}
}

// Assuming the polygon is in the counterclockwise direction
void Building::triangulatePolygon(const std::vector<glm::vec3>& points, std::vector<glutil::Vertex>& vertices, std::vector<GLuint>& indices, float height, float centroidHeightOffset)
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
	centroid.y += centroidHeightOffset;
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


