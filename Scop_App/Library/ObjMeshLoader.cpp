#include "ObjMeshLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../Includes/Common/Vertex.hpp"

namespace vks {

	ObjMeshLoader::ObjMeshLoader(const std::string& objFilepath)
	{
		if (objFilepath.empty())
		{
			throw std::runtime_error(std::string("filename is wrong"));

		}

		std::string mtlFilename;

		std::ifstream file;
		
		checkFile(objFilepath, ".obj");
		std::string Filepath = "./Scop_App/models/" + objFilepath;
		std::string Filepath2 = "./Scop_App/models/42.obj";
		std::cout << "Filepath :" << Filepath << std::endl;
		std::cout << "Filepath2 :" << Filepath2 << std::endl;
		file.open(Filepath2);
		if (!file.is_open()) {
			throw std::runtime_error(std::string("Failed to open OBJ file: ") + Filepath2);
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		if (file.fail()) {
			file.close();
			throw std::runtime_error(std::string("Failed to read OBJ file: ") + objFilepath);
		}
		file.close();

		std::string line;
		std::vector<std::string> words;

		while (std::getline(buffer, line)) {
			std::string delimiter = " ";
			words = split(line, delimiter);

			if (!words[0].compare("mtllib")) {
				// save mtl
				mtlFilename = words[1];
				std::cout << "MTL file name : " << mtlFilename << std::endl;
				parseMtlFile(mtlFilename);
			}

			if (!words[0].compare("v")) {
				read_vertex_data(words);
			}
			if (!words[0].compare("vt")) {
				read_texcoord_data(words);
			}
			if (!words[0].compare("vn")) {
				read_normal_data(words);
			}
			if (!words[0].compare("usemtl")) {
				//std::cout << words[1] << std::endl;
				if (colorLookup.contains(words[1])) {
					brushColor = colorLookup[words[1]];
				}
				else {
					brushColor = mymath::Vec3(-1.0f);
				}
			}

			if (!words[0].compare("f")) {
				read_face_data(words);
			}
		}
		file.close();

		size_t vertexCount = vertices.size();
		mymath::Vec3 center = sum / (float)vertexCount;

		uint32_t triangleID = 0;
		for (auto& vertex : vertices) {
			vertex.position[0] -= center._x;
			vertex.position[1] -= center._y;
			vertex.position[2] -= center._z;

			vertex.triangleID = triangleID++;
		}
		


	}

	void ObjMeshLoader::checkFile(const std::string& path, const std::string& extension)
	{
		/*	if (path.empty()) {
				throw std::invalid_argument("Empty file name");
			}*/
		std::size_t	extension_pos = path.rfind('.');

		if (extension_pos == std::string::npos) {
			throw std::invalid_argument("File '" + path + "' has no extension");
		}
		else if (path.substr(extension_pos) != extension) {
			throw std::invalid_argument("File '" + path + "' is not a ." + extension + "file");
		}
	}

	void ObjMeshLoader::parseMtlFile(std::string& mtlFilename)
	{
		if (!mtlFilename.empty())
		{
			// Ȯ���� üũ .mtl
			checkFile(mtlFilename, ".mtl");

			mtlFilename = "./Scop_App/models/" + mtlFilename;
			std::ifstream file;
			file.open(mtlFilename);
			if (!file.is_open()) {
				std::cerr << "Failed to open material file: " << mtlFilename << std::endl;
				return;
				//throw std::runtime_error(std::string("Failed to open material file: ") + mtlFilename);
			}

			std::stringstream buffer;
			buffer << file.rdbuf();
			if (file.fail()) {
				throw std::runtime_error(std::string("Failed to read material file: ") + mtlFilename);

			}
			file.close();

			std::string line;
			std::string materialName;
			std::vector<std::string> words;

			while (std::getline(buffer, line)) {

				std::string delimiter = " ";
				words = split(line, delimiter);

				if (!words[0].compare("newmtl")) {
					materialName = words[1];
				}
				if (!words[0].compare("Kd")) {
					brushColor = mymath::Vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
					colorLookup.insert({ materialName, brushColor });
				}
			}


		}
	}

	void ObjMeshLoader::read_vertex_data(const std::vector<std::string>& words) {
		mymath::Vec3 new_vertex = mymath::Vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
		v.push_back(new_vertex);

		minX = std::min(new_vertex._x, minX);
		minY = std::min(new_vertex._y, minY);
		minZ = std::min(new_vertex._z, minZ);

		maxX = std::max(new_vertex._x, maxX);
		maxY = std::max(new_vertex._y, maxY);
		maxZ = std::max(new_vertex._z, maxZ);

		sum += new_vertex;

		if (words.size() > 6) {
			mymath::Vec3 color = mymath::Vec3(std::stof(words[4]), std::stof(words[5]), std::stof(words[6]));
			vertexColors.push_back(color);
		}
		else {
			vertexColors.push_back(mymath::Vec3(-1.0f)); // �⺻ ����
		}
	}

	void ObjMeshLoader::read_texcoord_data(const std::vector<std::string>& words) {
		mymath::Vec2 new_texcoord = mymath::Vec2(std::stof(words[1]), std::stof(words[2]));
		vt.push_back(new_texcoord);
	}

	void ObjMeshLoader::read_normal_data(const std::vector<std::string>& words) {
		mymath::Vec3 new_normal = mymath::Vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
		vn.push_back(new_normal);
	}

	void ObjMeshLoader::read_face_data(const std::vector<std::string>& words) {

		size_t triangleCount = words.size() - 3;

		for (size_t i = 0; i < triangleCount; ++i) {					
			read_corner(words[1]);
			read_corner(words[2 + i]);
			read_corner(words[3 + i]);					
		}
	}

	void ObjMeshLoader::read_corner(const std::string& vertex_description) {
		auto it = history.find(vertex_description);
		if (it != history.end()) {
			indices.push_back(it->second);
			return;
		}

		uint32_t index = static_cast<uint32_t>(history.size());
		history.insert({ vertex_description, index });
		indices.push_back(index);
		//triangle = index;

		std::vector<std::string> v_vt_vn = split(vertex_description, "/");

		::ScopVertex vertex{};

		//position
		mymath::Vec3 pos = v[std::stol(v_vt_vn[0]) - 1];
		vertex.position[0] = pos._x;
		vertex.position[1] = pos._y;
		vertex.position[2] = pos._z;


		// color
		mymath::Vec3 color = vertexColors[std::stol(v_vt_vn[0]) - 1];
		if (color == mymath::Vec3(-1.0f)) { // �⺻ ������ ���?�������� ���?

			// color
			if (brushColor == mymath::Vec3(-1.0f)) { // �⺻ ������ ���?�������� ���?
				static const mymath::Vec3 rainbowColors[] = {
					{1.0f, 0.0f, 0.0f}, // ����
					{1.0f, 0.5f, 0.0f}, // ��Ȳ
					{1.0f, 1.0f, 0.0f}, // ���?
					{0.0f, 1.0f, 0.0f}, // �ʷ�
					{0.0f, 0.0f, 1.0f}, // �Ķ�
					{0.29f, 0.0f, 0.51f}, // ����
					{0.56f, 0.0f, 1.0f}  // ����
				};
				size_t colorIndex = index % (sizeof(rainbowColors) / sizeof(rainbowColors[0]));
				vertex.color[0] = rainbowColors[colorIndex]._x;
				vertex.color[1] = rainbowColors[colorIndex]._y;
				vertex.color[2] = rainbowColors[colorIndex]._z;
			}
			else {
				vertex.color[0] = brushColor._x;
				vertex.color[1] = brushColor._y;
				vertex.color[2] = brushColor._z;
			}
		}
		else {
			vertex.color[0] = color._x;
			vertex.color[1] = color._y;
			vertex.color[2] = color._z;
		}

		
		
		

		//texcoord
		mymath::Vec2 texcoord = mymath::Vec2(0.0f, 0.0f);
		if (v_vt_vn.size() > 1 && !v_vt_vn[1].empty()) {
			texcoord = vt[std::stol(v_vt_vn[1]) - 1];
		}
		if (!vt.empty())
		{
			vertex.uv[0] = texcoord._x;
			vertex.uv[1] = texcoord._y;
		}
		else
		{
			//vertex.uv[0] = (vertex.position[0] - minX) / (maxX - minX);			
			//vertex.uv[1] = 1.0f - ((vertex.position[1] - minY) / (maxY - minY));	
			
			float theta = std::atan2(vertex.position[2], vertex.position[0]);
			float sqrtXYZ = std::sqrt(vertex.position[0] * vertex.position[0] + vertex.position[1] * vertex.position[1] + vertex.position[2] * vertex.position[2]);
			float phi = std::acos(vertex.position[1] / sqrtXYZ);
			vertex.uv[0] = (theta + 3.14159265358979323846f) / (2.0f * 3.14159265358979323846f);
			vertex.uv[1] = phi / 3.14159265358979323846f;
		}
			

		//normal
		mymath::Vec3 normal = mymath::Vec3(0.0f, 0.0f, 0.0f); // �⺻�� ����
		if (v_vt_vn.size() > 2 && !v_vt_vn[2].empty()) {
			normal = vn[std::stol(v_vt_vn[2]) - 1];
		}
		vertex.normal[0] = normal._x;
		vertex.normal[1] = normal._y;
		vertex.normal[2] = normal._z;

		
		vertices.push_back(vertex);
	}

	std::vector<std::string> ObjMeshLoader::split(const std::string& line, const std::string& delimiter)
	{
		std::vector<std::string> split_line;
		size_t start = 0;
		size_t end = line.find(delimiter);

		while (end != std::string::npos) {
			if (end != start) { // ���ӵ� �����ڸ� ����
				split_line.push_back(line.substr(start, end - start));
			}
			start = end + delimiter.length();
			end = line.find(delimiter, start);
		}
		split_line.push_back(line.substr(start));

		return split_line;
	}
}

