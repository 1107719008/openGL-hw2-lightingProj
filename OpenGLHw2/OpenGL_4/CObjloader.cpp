#include "CObjloader.h"

CObjloader::CObjloader(const char* fileName)
{
	//_iNumVtx = OBJLOADER_NUM;
	_pPoints = NULL; _pNormals = NULL; _pTex = NULL;

	
	/*std::vector<vec4>_pPoints;
	std::vector<vec2>_pNormals;
	std::vector<vec3>_pTex;*/

	LoadObj(fileName);

	_iIndex = 0;
	/*_pPoints  = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors  = new vec4[_iNumVtx]; 
	_pTex     = new vec2[_iNumVtx];*/

	

   /* _vertices[0] = point4( -0.5, -0.5,  0.5, 1.0 );
    _vertices[1] = point4( -0.5,  0.5,  0.5, 1.0 );
    _vertices[2] = point4(  0.5,  0.5,  0.5, 1.0 );
    _vertices[3] = point4(  0.5, -0.5,  0.5, 1.0 );
    _vertices[4] = point4( -0.5, -0.5, -0.5, 1.0 );
    _vertices[5] = point4( -0.5,  0.5, -0.5, 1.0 );
    _vertices[6] = point4(  0.5,  0.5, -0.5, 1.0 );
	_vertices[7] = point4(  0.5, -0.5, -0.5, 1.0 );*/

	
	// generate 12 triangles: 36 vertices and 36 colors
 /*   Quad( 1, 0, 3, 2 );
    Quad( 2, 3, 7, 6 );
    Quad( 3, 0, 4, 7 );
    Quad( 6, 5, 1, 2 );
    Quad( 4, 5, 6, 7 );
    Quad( 5, 4, 0, 1 );*/

	//// 預設將所有的面都設定成灰色
	for( int i = 0 ; i < _iNumVtx ; i++ ) _pColors[i] = vec4(-1.0f,-1.0f,-1.0f,1.0f);

	// 設定材質
	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	setKaKdKsShini(0, 0.8f, 0.2f, 1);
}

void CObjloader::Quad( int a, int b, int c, int d )
{
    // Initialize temporary vectors along the quad's edge to
    //   compute its face normal 
    vec4 u = _vertices[b] - _vertices[a];
    vec4 v = _vertices[c] - _vertices[b];
    vec3 normal = normalize( cross(u, v) );

    _pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[a]; _iIndex++;
    _pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[b]; _iIndex++;
    _pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[c]; _iIndex++;
    _pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[a]; _iIndex++;
    _pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[c]; _iIndex++;
    _pNormals[_iIndex] = normal; _pPoints[_iIndex] = _vertices[d]; _iIndex++;
}

void CObjloader::draw()
{
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	drawingSetShader();
	glDrawArrays( GL_TRIANGLES, 0, _iNumVtx);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}

void CObjloader::drawW()
{
	drawingWithoutSetShader();
	glDrawArrays( GL_TRIANGLES, 0, _iNumVtx);
}
// 此處所給的 vLightPos 必須是世界座標的確定絕對位置
void CObjloader::update(float dt, point4 vLightPos, color4 vLightI)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(vLightPos, vLightI);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(vLightPos, vLightI);
	else {
		_vLightInView = _mxView * vLightPos;	// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * vLightI;
		_DiffuseProduct = _Material.kd * _Material.diffuse * vLightI;
		_SpecularProduct = _Material.ks * _Material.specular * vLightI;
	}
}

void CObjloader::update(float dt, const LightSource& Lights)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Lights);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Lights);
	else {
		_vLightInView = _mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * Lights.ambient;
		_DiffuseProduct = _Material.kd * _Material.diffuse * Lights.diffuse;
		_SpecularProduct = _Material.ks * _Material.specular * Lights.specular;
	}
}

void CObjloader::update(float dt, const LightSource& Lights, const LightSource& Lights2, const LightSource& Lights3, const LightSource& Lights4)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Lights);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Lights);
	else {
		_vLightInView = _mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入 shader
		_vLightInView2 = _mxView * Lights2.position;
		_vLightInView3 = _mxView * Lights3.position;
		_vLightInView4 = _mxView * Lights4.position;
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * Lights.ambient;
		_AmbientProduct2 = _Material.ka * _Material.ambient * Lights2.ambient;
		_AmbientProduct3 = _Material.ka * _Material.ambient * Lights3.ambient;
		_AmbientProduct4 = _Material.ka * _Material.ambient * Lights4.ambient;

		_DiffuseProduct = _Material.kd * _Material.diffuse * Lights.diffuse;
		_DiffuseProduct2 = _Material.kd * _Material.diffuse * Lights2.diffuse;
		_DiffuseProduct3 = _Material.kd * _Material.diffuse * Lights3.diffuse;
		_DiffuseProduct4 = _Material.kd * _Material.diffuse * Lights4.diffuse;

		_SpecularProduct = _Material.ks * _Material.specular * Lights.specular;
		_SpecularProduct2 = _Material.ks * _Material.specular * Lights2.specular;
		_SpecularProduct3 = _Material.ks * _Material.specular * Lights3.specular;
		_SpecularProduct4 = _Material.ks * _Material.specular * Lights4.specular;
	}
}

// 呼叫沒有給光源的 update 代表該物件不會進行光源照明的計算
void CObjloader::update(float dt)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算
}


void CObjloader::LoadObj(const char* fileName)
{
	int vertex_Num=0;
	char lineHeader[3000] = {0};

	FILE* ObjFile = fopen(fileName, "r");
	if (ObjFile == NULL)printf("000");
	else {
		while (fscanf(ObjFile, "%s", lineHeader) != EOF) {
			if ((strcmp(lineHeader, "f") == 0))	vertex_Num++;
		}
		fclose(ObjFile);
	}

	_iNumVtx = vertex_Num*3;
	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];
	_pTex = new vec2[_iNumVtx];
	vertex_Num = 0;

	// array
	/*vec4* temp_vertices = new vec4[_iNumVtx];
	vec2* temp_uvs = new vec2[_iNumVtx];
	vec3* temp_normals = new vec3[_iNumVtx];*/

	std::vector<GLuint>vertexIndices, uvIndices, normalIndices;
	std::vector<vec4>temp_vertices;
	std::vector<vec2>temp_uvs;
	std::vector<vec3>temp_normals;


	/*int index_vertex = 0;
	int index_normal = 0;
	int index_tex = 0;
	int index_fPoly = 0;*/

	/*unsigned int vertexIndices;
	unsigned int uvIndices;
	unsigned int normalIndices;*/

	/*float vertex[3];
	int face[10];*/

	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		
	}
	else {
		while (fscanf(file, "%s", lineHeader) != EOF) {

			// read the first word of the line

			if (strcmp(lineHeader, "v") == 0) {

				//fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				////temp_vertices.push_back(vertex);
				//temp_vertices[index_vertex] = vec4(vertex[0], vertex[1], vertex[2],1);
				//index_vertex++;

				vec4 vertex;

				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				//temp_vertices.push_back(vertex.x);
				//temp_vertices.push_back(vertex.y);
				//temp_vertices.push_back(vertex.z);
				vertex.w = 1;
				temp_vertices.push_back(vertex);
			

			}
			else if (strcmp(lineHeader, "vt") == 0) {
				//float uv[2];
				//fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				////temp_uvs.push_back(uv);
				//temp_uvs[index_tex] = vec2(uv[0], uv[1]);
				//index_tex++;

				/*fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_uvs[index_tex] = vec2(vertex[0], vertex[1]);
				index_tex++;*/

				vec2 uv;
				fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
				//temp_uvs.push_back(uv.y);

			}
			else if (strcmp(lineHeader, "vn") == 0) {
				//float normal[3];
				//fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				////temp_normals.push_back(normal);
				//temp_normals[index_normal] = vec3(normal[0], normal[1], normal[2]);
				//index_normal++;

				/*fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_normals[index_normal] = vec3(vertex[0], vertex[1], vertex[2]);
				index_normal++;*/

				vec3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
				//temp_normals.push_back(normal.y);
				//temp_normals.push_back(normal.z);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				//std::string vertex1, vertex2, vertex3;
				/*unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");

				}

				for (int i = 0; i < res; i++)
				{

					_pPoints[index_fPoly] = vertexIndex[i];
					_pNormals[index_fPoly] = uvIndex[i];
					_pTex[index_fPoly] = normalIndex[i];
					if (i == 3) {
						i = 0;
					}
				}*/

				/*fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2], &face[3], &face[4], &face[5], &face[6], &face[7], &face[8]);
				_pPoints[vertex_Num] = temp_vertices[face[0] - 1];
				_pPoints[vertex_Num + 1] = temp_vertices[face[3] - 1];
				_pPoints[vertex_Num + 2] = temp_vertices[face[6] - 1];

				_pTex[vertex_Num] = temp_uvs[face[1] - 1];
				_pTex[vertex_Num + 1] = temp_uvs[face[4] - 1];
				_pTex[vertex_Num + 2] = temp_uvs[face[7] - 1];

				_pNormals[vertex_Num] = temp_normals[face[2] - 1];
				_pNormals[vertex_Num + 1] = temp_normals[face[5] - 1];
				_pNormals[vertex_Num + 2] = temp_normals[face[8] - 1];
				vertex_Num = vertex_Num + 3;
				*/

				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other optionsn");

				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

			}

		}
		fclose(file);

		for (int i = 0; i < _iNumVtx; i++) {
			_pColors[i] = (vec4(1.0f, 1.0f, 1.0f, 1.0f));

		}
		for (unsigned int i = 0; i < _iNumVtx; i++)
		{
			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			
			_pPoints[i] = temp_vertices[vertexIndex-1];
			_pPoints[i].w = 1;
			_pTex[i] = temp_uvs[uvIndex-1];
			_pNormals[i] = temp_normals[normalIndex -1];
			

			Print(_pPoints[i]);
			Print(_pTex[i]);
			Print(_pNormals[i]);
			

		}
		Print(_iNumVtx);
		
	}
	Print("loaded");
}
//-----------------------





//
//void CObjloader::renderWithFlatShading(vec4 vLightPos, color4 vLightI)
//{
//	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
//	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
//	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal
//
//	vec4 vCentroidP;
//	for( int i = 0 ; i < _iNumVtx ; i += 3 ) {
//		// 計算三角形的重心
//		vCentroidP = (_pPoints[i] + _pPoints[i+1] + _pPoints[i+2])/3.0f;
//		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = PhongReflectionModel(vCentroidP, _pNormals[i], vLightPos, vLightI);
//	}
//	glBindBuffer( GL_ARRAY_BUFFER, _uiBuffer );
//	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*_iNumVtx+sizeof(vec3)*_iNumVtx, sizeof(vec4)*_iNumVtx, _pColors ); // vertcies' Color
//}
//
//void CObjloader::renderWithFlatShading(const LightSource &Lights)
//{
//	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
//	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
//	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal
//
//	vec4 vCentroidP;
//	for (int i = 0; i < _iNumVtx; i += 3) {
//		// 計算三角形的重心
//		vCentroidP = (_pPoints[i] + _pPoints[i + 1] + _pPoints[i + 2]) / 3.0f;
//		_pColors[i] = _pColors[i + 1] = _pColors[i + 2] = PhongReflectionModel(vCentroidP, _pNormals[i], Lights.position, Lights.diffuse);
//	}
//	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*_iNumVtx + sizeof(vec3)*_iNumVtx, sizeof(vec4)*_iNumVtx, _pColors); // vertcies' Color
//}
//
//void CObjloader::renderWithGouraudShading(vec4 vLightPos, color4 vLightI)
//{
//	vec4 vCentroidP;
//	for( int i = 0 ; i < _iNumVtx ; i += 6 ) {
//		_pColors[i] = _pColors[i + 3] = PhongReflectionModel(_pPoints[i], _pNormals[i], vLightPos, vLightI);
//		_pColors[i + 2] = _pColors[i + 4] = PhongReflectionModel(_pPoints[i + 2], _pNormals[i + 2], vLightPos, vLightI);
//		_pColors[i + 1] = PhongReflectionModel(_pPoints[i + 1], _pNormals[i + 1], vLightPos, vLightI);
//		_pColors[i + 5] = PhongReflectionModel(_pPoints[i + 5], _pNormals[i + 5], vLightPos, vLightI);
//	}
//	glBindBuffer( GL_ARRAY_BUFFER, _uiBuffer );
//	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*_iNumVtx+sizeof(vec3)*_iNumVtx, sizeof(vec4)*_iNumVtx, _pColors ); // vertcies' Color
//}
//
//void CObjloader::renderWithGouraudShading(const LightSource &Lights)
//{
//	vec4 vCentroidP;
//	for (int i = 0; i < _iNumVtx; i += 6) {
//		_pColors[i] = _pColors[i + 3] = PhongReflectionModel(_pPoints[i], _pNormals[i], Lights);
//		_pColors[i + 2] = _pColors[i + 4] = PhongReflectionModel(_pPoints[i + 2], _pNormals[i + 2], Lights);
//		_pColors[i + 1] = PhongReflectionModel(_pPoints[i + 1], _pNormals[i + 1], Lights);
//		_pColors[i + 5] = PhongReflectionModel(_pPoints[i + 5], _pNormals[i + 5], Lights);
//	}
//	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*_iNumVtx + sizeof(vec3)*_iNumVtx, sizeof(vec4)*_iNumVtx, _pColors); // vertcies' Color
//}