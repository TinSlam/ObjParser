#include "ObjParserModel.h"

extern void parseMaterialsFile(std::string path);

ObjParserModel::ObjParserModel(std::string fullPath){
	this->fullPath = fullPath;
}

void ObjParserModel::vertexFound(){
	float x = (float) atof(strList[0].c_str());
	float y = (float) atof(strList[1].c_str());
	float z = (float) atof(strList[2].c_str());
	if(x < minX) minX = x;
	if(x > maxX) maxX = x;
	if(y < minY) minY = y;
	if(y > maxY) maxY = y;
	if(z < minZ) minZ = z;
	if(z > maxZ) maxZ = z;
	vertices.push_back(new float[3]{x, y, z});
}

void ObjParserModel::textureCoordsFound(){
	textureCoordinates.push_back(new float[2]{
		(float) atof(strList[0].c_str()),
		1 - (float) atof(strList[1].c_str())
	});
}

void ObjParserModel::normalsFound(){
	normals.push_back(new float[3]{
		(float) atof(strList[0].c_str()),
		(float) atof(strList[1].c_str()),
		(float) atof(strList[2].c_str())
	});
}

void ObjParserModel::faceFound(){
	short index1 = (short) atoi(vertex1List[0].c_str());
	short index2 = (short) atoi(vertex2List[0].c_str());
	short index3 = (short) atoi(vertex3List[0].c_str());
	if(newFace){
		lastFaces[0] = index1;
		lastFaces[1] = index2;
		lastFaces[2] = index3;
		if(std::find(verticesFoundSoFar.begin(), verticesFoundSoFar.end(), index1) != verticesFoundSoFar.end()){
			vertices.push_back(vertices[index1 - 1]);
			index1 = (short) vertices.size();
		}else{
			verticesFoundSoFar.push_back(index1);
		}
		if(std::find(verticesFoundSoFar.begin(), verticesFoundSoFar.end(), index2) != verticesFoundSoFar.end()){
			vertices.push_back(vertices[index2 - 1]);
			index2 = (short) vertices.size();
		}else{
			verticesFoundSoFar.push_back(index2);
		}
		if(std::find(verticesFoundSoFar.begin(), verticesFoundSoFar.end(), index3) != verticesFoundSoFar.end()){
			vertices.push_back(vertices[index3 - 1]);
			index3 = (short) vertices.size();
		}else{
			verticesFoundSoFar.push_back(index3);
		}
		lastFacesNewValue[0] = index1;
		lastFacesNewValue[1] = index2;
		lastFacesNewValue[2] = index3;
	}else{
		int ind = 0;
		bool flag = false;
		for(int i = 0; i < 3; i++){
			if(index1 == lastFaces[i]){
				flag = true;
				ind = i;
				break;
			}
		}
		if(!flag){
			if(std::find(verticesFoundSoFar.begin(), verticesFoundSoFar.end(), index1) != verticesFoundSoFar.end()){
				vertices.push_back(vertices[index1 - 1]);
				index1 = (short) vertices.size();
			}else{
				verticesFoundSoFar.push_back(index1);
			}
		}else{
			index1 = lastFacesNewValue[ind];
		}
		flag = false;
		for(int i = 0; i < 3; i++){
			if(index2 == lastFaces[i]){
				flag = true;
				ind = i;
				break;
			}
		}
		if(!flag){
			if(std::find(verticesFoundSoFar.begin(), verticesFoundSoFar.end(), index2) != verticesFoundSoFar.end()){
				vertices.push_back(vertices[index2 - 1]);
				index2 = (short) vertices.size();
			}else{
				verticesFoundSoFar.push_back(index2);
			}
		}else{
			index2 = lastFacesNewValue[ind];
		}
		flag = false;
		for(int i = 0; i < 3; i++){
			if(index3 == lastFaces[i]){
				flag = true;
				ind = i;
				break;
			}
		}
		if(!flag){
			if(std::find(verticesFoundSoFar.begin(), verticesFoundSoFar.end(), index3) != verticesFoundSoFar.end()){
				vertices.push_back(vertices[index3 - 1]);
				index3 = (short) vertices.size();
			}else{
				verticesFoundSoFar.push_back(index3);
			}
		}else{
			index3 = lastFacesNewValue[ind];
		}
	}
	newFace = !newFace;
	faces.push_back(new short[9]{
		index1,
		(short) atoi(vertex1List[1].c_str()),
		(short) atoi(vertex1List[2].c_str()),
		index2,
		(short) atoi(vertex2List[1].c_str()),
		(short) atoi(vertex2List[2].c_str()),
		index3,
		(short) atoi(vertex3List[1].c_str()),
		(short) atoi(vertex3List[2].c_str())
	});
}

void ObjParserModel::wrapUp(){
	vbo = (float*) calloc(static_cast<size_t>(8 * faces.size() * 3), sizeof(float));
	ibo = (short*) calloc(static_cast<size_t>(faces.size() * 3), sizeof(short));

	for(size_t i = 0; i < faces.size(); i++){
		(ibo)[i * 3] = (short) (faces[i][0] - 1);
		(vbo)[(int) (ibo)[i * 3] * 8] = vertices[(int) faces[i][0] - 1][0];
		(vbo)[(int) (ibo)[i * 3] * 8 + 1] = vertices[(int) faces[i][0] - 1][1];
		(vbo)[(int) (ibo)[i * 3] * 8 + 2] = vertices[(int) faces[i][0] - 1][2];
		(vbo)[(int) (ibo)[i * 3] * 8 + 6] = textureCoordinates[(int) faces[i][1] - 1][0];
		(vbo)[(int) (ibo)[i * 3] * 8 + 7] = textureCoordinates[(int) faces[i][1] - 1][1];
		(ibo)[i * 3 + 1] = (short) (faces[i][3] - 1);
		(vbo)[(int) (ibo)[i * 3 + 1] * 8] = vertices[(int) faces[i][3] - 1][0];
		(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 1] = vertices[(int) faces[i][3] - 1][1];
		(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 2] = vertices[(int) faces[i][3] - 1][2];
		(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 6] = textureCoordinates[(int) faces[i][4] - 1][0];
		(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 7] = textureCoordinates[(int) faces[i][4] - 1][1];
		(ibo)[i * 3 + 2] = (short) (faces[i][6] - 1);
		(vbo)[(int) (ibo)[i * 3 + 2] * 8] = vertices[(int) faces[i][6] - 1][0];
		(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 1] = vertices[(int) faces[i][6] - 1][1];
		(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 2] = vertices[(int) faces[i][6] - 1][2];
		(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 6] = textureCoordinates[(int) faces[i][7] - 1][0];
		(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 7] = textureCoordinates[(int) faces[i][7] - 1][1];
		if((int) faces[i][2] - 1 < normals.size()){
			(vbo)[(int) (ibo)[i * 3] * 8 + 3] = normals[(int) faces[i][2] - 1][0];
			(vbo)[(int) (ibo)[i * 3] * 8 + 4] = normals[(int) faces[i][2] - 1][1];
			(vbo)[(int) (ibo)[i * 3] * 8 + 5] = normals[(int) faces[i][2] - 1][2];
			(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 3] = normals[(int) faces[i][5] - 1][0];
			(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 4] = normals[(int) faces[i][5] - 1][1];
			(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 5] = normals[(int) faces[i][5] - 1][2];
			(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 3] = normals[(int) faces[i][8] - 1][0];
			(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 4] = normals[(int) faces[i][8] - 1][1];
			(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 5] = normals[(int) faces[i][8] - 1][2];
		}else{
			(vbo)[(int) (ibo)[i * 3] * 8 + 3] = 0;
			(vbo)[(int) (ibo)[i * 3] * 8 + 4] = 0;
			(vbo)[(int) (ibo)[i * 3] * 8 + 5] = 0;
			(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 3] = 0;
			(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 4] = 0;
			(vbo)[(int) (ibo)[i * 3 + 1] * 8 + 5] = 0;
			(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 3] = 0;
			(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 4] = 0;
			(vbo)[(int) (ibo)[i * 3 + 2] * 8 + 5] = 0;
		}
	}

	width = maxX - minX;
	height = maxY - minY;
	depth = maxZ - minZ;

	vboLength = 8 * (int) faces.size() * 3;
	iboLength = (int) faces.size() * 3;

	materialsIndicesVector.push_back((int) faces.size());

	materials = new int[materialsVector.size()];
	materialsIndices = new int[materialsIndicesVector.size()];
	materialsCount = (int) materialsVector.size();

	int i = 0;
	for(auto it = materialsVector.begin(); it != materialsVector.end(); it++){
		*(materials + i) = *it;
		i++;
	}
	i = 0;
	for(auto it = materialsIndicesVector.begin(); it != materialsIndicesVector.end(); it++){
		*(materialsIndices + i) = *it;
		i++;
	}
}

void ObjParserModel::newMaterial(std::string material){
	idMap[material] = currentMaterial;
}

void ObjParserModel::mapKd(std::string path){
	(*materialsPath)[currentMaterial++] = path;
}

void ObjParserModel::parseMtlFile(std::string fileName){
	int index = 0;
	for(int i = 0; i < fullPath.length(); i++){
		if(fullPath[i] == '/')
			index = i;
	}
	std::string relativeDirectoryPath = fullPath.substr(0, index);
	parseMaterialsFile(relativeDirectoryPath + "/" + fileName);
}

void ObjParserModel::setCurrentMaterial(std::string name){
	materialsIndicesVector.push_back((int) faces.size());
	materialsVector.push_back(idMap[name]);
}