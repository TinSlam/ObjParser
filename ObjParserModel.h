#include <vector>
#include <map>

class ObjParserModel{
	private :
		std::vector<float*> vertices;
		std::vector<float*> textureCoordinates;
		std::vector<float*> normals;
		std::vector<short*> faces;
		std::vector<short> verticesFoundSoFar;
		
		bool newFace = true;
		short lastFaces[3] = { 0, 0, 0 };
		short lastFacesNewValue[3] = { 0, 0, 0 };

		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		std::map<std::string, int> idMap;
		std::vector<int> materialsIndicesVector;
		std::vector<int> materialsVector;

		int currentMaterial = 1;

		std::string fullPath;

	public :
		ObjParserModel(std::string path);

		std::vector<std::string> strList = {"", "", "", ""};
		std::vector<std::string> vertex1List = {"", "", ""};
		std::vector<std::string> vertex2List = {"", "", ""};
		std::vector<std::string> vertex3List = {"", "", ""};

		float* vbo = nullptr;
		int vboLength = 0;
		short* ibo = nullptr;
		int iboLength = 0;
		float width = 0;
		float height = 0;
		float depth = 0;
		int* materialsIndices = nullptr;
		int* materials = nullptr;
		std::map<int, std::string>* materialsPath = nullptr;
		int materialsCount = 0;

		void vertexFound();
		void textureCoordsFound();
		void normalsFound();
		void faceFound();
		void wrapUp();

		void parseMtlFile(std::string path);
		void setCurrentMaterial(std::string mat);
		void mapKd(std::string material);
		void newMaterial(std::string material);
};