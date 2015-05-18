//
// Created by David on 2015-04-14.
//
// Component for model and shader

#include <Model.h>

#include <GameObject.h>

static const char MODEL_REL_PATH[] = TSBK07_MODELS_PATH;
static const char SHADER_REL_PATH[] = TSBK07_SHADERS_PATH;


Model::Model(const std::string& jsonPath) :
    BaseComponent(), jsonLoader(jsonPath) {

    jsonLoader.AddDataField<std::string>("model", &model);
	jsonLoader.AddArrayField<float>("color", colors, 4);
    //jsonLoader.AddDataField<std::string>("vertshader", &vertshader);
	//jsonLoader.AddDataField<std::string>("fragshader", &fragshader);
    jsonLoader.Read();

	init(model.c_str());
}

Model::Model(const char *model) : BaseComponent()
{
    init(model);
}

void Model::init(const char *model) {
    std::string _model(MODEL_REL_PATH);
   // std::string vertShader(SHADER_REL_PATH);
    //std::string fragShader(SHADER_REL_PATH);

    // Get full object file path
    _model.append("/");
    _model.append(model);
    _model.append(".obj");

    // Load the model.
    LoadObject(_model.c_str());

    // Get full shader paths
    /*vertShader.append("/");
    fragShader.append("/");
	vertShader.append(vertexshader);
	fragShader.append(fragmentshader);
    // TODO: Either use this convention or something else 
    // e.g. keep separate entries in JSON.
    vertShader.append(".glsl");
    fragShader.append(".glsl");*/
}

Model::~Model()
{
    // JSONLoader will automatically update JSON file on destruct
}



void Model::LoadObject(const char* filename)
{

    std::vector<std::string> coord; // Each line read from file
    std::vector<Vector3f> vertex;
	std::vector<Vector3f> normal;
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "Model file could not be opened" << std::endl;
        return;
    }

	// Reserve enough space in coord so we don't get 9000+ reallocations
	long line_count = std::count(std::istreambuf_iterator<char>(in),
			   std::istreambuf_iterator<char>(), '\n') + 1;
	if (line_count > 0) {
		coord.reserve(static_cast<unsigned long>(line_count));
	}
	in.seekg(0, std::ios_base::beg);

    char buf[256];
    // Read in every line to coord
    while (!in.eof())
    {
        in.getline(buf, 256);
        coord.emplace_back(buf);
    }

	std::cout << "Model: Loading " << coord.size() << " vertices/normals/triangles/quads" << std::endl;

    // Go through all elements of the coord and decide what kind of element it is
    for (size_t i = 0; i < coord.size(); i++) 
    {
		std::cout << "Model: Line " << i << ": " << coord[i] << std::endl;
        if (coord[i].c_str()[0] == '#')   // Comment
            continue;      
        else if (coord[i].c_str()[0] == 'v' && coord[i].c_str()[1] == ' ') // Vertice
        {
			GLfloat tmpx, tmpy, tmpz;
            sscanf(coord[i].c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz); // Read in the 3 float coordinate to tmpx,tmpy,tmpz
			vertex.push_back(Vector3f(tmpx, tmpy, tmpz));       // and then add it to the end of our vertex list
		
        }
        else if (coord[i].c_str()[0] == 'v' && coord[i].c_str()[1] == 'n') // Normal
        {
            float tmpx, tmpy, tmpz; 
            sscanf(coord[i].c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normal.push_back(Vector3f(tmpx, tmpy, tmpz));       // and then add it to the end of our vertex list

        }
        else if (coord[i].c_str()[0] == 'f') // Face
        {
			
            if (std::count(coord[i].begin(), coord[i].end(), ' ') == 3) // Check if triangle
            {
				unsigned int vertexIndex[3], normalIndex[3]; // uvIndex[3],
				if (6 == sscanf(coord[i].c_str(), "f %d//%d %d//%d %d//%d",
								  &vertexIndex[0], &normalIndex[0], &vertexIndex[1],
								  &normalIndex[1], &vertexIndex[2], &normalIndex[2])) {

					// Add vertices to final vertice list
					for (int n = 0; n < 3; n++) {
						if (vertexIndex[n] > vertex.size() ||
								normalIndex[n] > normal.size()) {
							std::cerr << "Model: Vertex/normal index is out of bounds" << std::endl;
							continue;
						}
						vertices.push_back(vertex[(vertexIndex[n] - 1)][0]);
						vertices.push_back(vertex[(vertexIndex[n] - 1)][1]);
						vertices.push_back(vertex[(vertexIndex[n] - 1)][2]);
						normals.push_back(normal[(normalIndex[n] - 1)][0]);
						normals.push_back(normal[(normalIndex[n] - 1)][1]);
						normals.push_back(normal[(normalIndex[n] - 1)][2]);
					}
				}
				else if (3 == sscanf(coord[i].c_str(), "f %d %d %d",
									 &vertexIndex[0], &vertexIndex[1], &vertexIndex[2])) {
					// Add vertices to final vertice list
					for (int n = 0; n < 3; n++) {
						if (vertexIndex[n] > vertex.size()) {
							std::cerr << "Model: Vertex index is out of bounds" << std::endl;
							continue;
						}
						vertices.push_back(vertex[(vertexIndex[n] - 1)][0]);
						vertices.push_back(vertex[(vertexIndex[n] - 1)][1]);
						vertices.push_back(vertex[(vertexIndex[n] - 1)][2]);
						Vector3f tmpNormal =
								(vertex[vertexIndex[n == 2 ? 0 : (n + 1)] -1] - vertex[(vertexIndex[n]) -1]).cross(
								(vertex[vertexIndex[n == 0 ? 2 : (n - 1)] -1] - vertex[(vertexIndex[n]) -1])).normalized();
						normals.push_back(tmpNormal[0]);
						normals.push_back(tmpNormal[1]);
						normals.push_back(tmpNormal[2]);
					}
				}
				else {
					std::cerr << "Model: Failed to parse vertex/normal" << std::endl;
				}
            }
            else{ // else quad
				
				unsigned int vertexIndex[4], normalIndex[4]; // uvIndex[3],
				if (EOF != sscanf(coord[i].c_str(), "f %d//%d %d//%d %d//%d %d//%d",
								  &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1],
								  &vertexIndex[2], &normalIndex[2], &vertexIndex[3], &normalIndex[3])) {

					for (int n = 0; n < 4; n++) {
						if (vertexIndex[n] > vertex.size() ||
							normalIndex[n] > normal.size()) {
							std::cerr << "Model: Vertex/normal index is out of bounds" << std::endl;
							continue;
						}
						vertices.push_back(vertex[(vertexIndex[n] - 1)][0]);
						vertices.push_back(vertex[(vertexIndex[n] - 1)][1]);
						vertices.push_back(vertex[(vertexIndex[n] - 1)][2]);
						normals.push_back(normal[(normalIndex[n] - 1)][0]);
						normals.push_back(normal[(normalIndex[n] - 1)][1]);
						normals.push_back(normal[(normalIndex[n] - 1)][2]);
					}
				}
				else {
					std::cerr << "Model: Failed to parse vertex/normal" << std::endl;
				}
            }
        }
		else {
			std::cout << "Model: Line couldn't be parsed" << std::endl;
		}
    }


}

void Model::AddTriangle(std::vector<GLfloat> *ver, Vector3f vec1, Vector3f vec2, Vector3f vec3){
	for (int n = 0; n < 3; n++){
		ver->push_back(vec1[n]);
	}
	for (int n = 0; n < 3; n++){
		ver->push_back(vec2[n]);
	}
	for (int n = 0; n < 3; n++){
		ver->push_back(vec3[n]);
	}
}

void Model::SplitTriangles() {
	bool patchingDone = false;
	std::vector<GLfloat> newVertices = vertices;
	std::vector<GLfloat> newNormals = normals;

	while (!patchingDone){

		std::vector<GLfloat> tmpVertices;
		std::vector<GLfloat> tmpNormals;
		Vector3f scale = GetTransform()->GetScale();

		//Split each triangle and add its vertices to newVertices

		for (int i = 0; i < newVertices.size() / 9; i++){
			// Get each vertice off triangle
			int verticeIdx = i * 9;
			Vector3f p1 = Vector3f(newVertices[0 + verticeIdx], newVertices[1 + verticeIdx], newVertices[2 + verticeIdx]);
			Vector3f p2 = Vector3f(newVertices[3 + verticeIdx], newVertices[4 + verticeIdx], newVertices[5 + verticeIdx]);
			Vector3f p3 = Vector3f(newVertices[6 + verticeIdx], newVertices[7 + verticeIdx], newVertices[8 + verticeIdx]);

			// Get normals for each vertice off triangle
			Vector3f n1 = Vector3f(newNormals[0 + verticeIdx], newNormals[1 + verticeIdx], newNormals[2 + verticeIdx]);
			Vector3f n2 = Vector3f(newNormals[3 + verticeIdx], newNormals[4 + verticeIdx], newNormals[5 + verticeIdx]);
			Vector3f n3 = Vector3f(newNormals[6 + verticeIdx], newNormals[7 + verticeIdx], newNormals[8 + verticeIdx]);

			// Get each side of the triangle
			Vector3f v1 = (p1 - p2);
			Vector3f v2 = (p1 - p3);
			Vector3f v3 = (p2 - p3);

			// Take scale in consideration when deciding what triangle sides are the longest
			Vector3f sv1 = Vector3f(scale.x()*v1.x(), scale.y()*v1.y(), scale.z()*v1.z());
			Vector3f sv2 = Vector3f(scale.x()*v2.x(), scale.y()*v2.y(), scale.z()*v2.z());
			Vector3f sv3 = Vector3f(scale.x()*v3.x(), scale.y()*v3.y(), scale.z()*v3.z());

			// Only split if triangle area is greater than MIN_PATCH_AREA
			if (sv1.norm()*sv2.norm() >= MIN_PATCH_AREA){
				patchingDone = false;
				// Get the longest side
				Vector3f splitPoint;
				if (sv1.squaredNorm() > sv2.squaredNorm()){
					if (sv1.squaredNorm() > sv3.squaredNorm()){
						// v1 longest
						splitPoint = p1 - v1 / 2;
						// New triangle 1
						AddTriangle(&tmpVertices, p1, splitPoint, p3);

						// New triangle 2
						AddTriangle(&tmpVertices, p2, splitPoint, p3);
					}
					else{
						// v3 longest
						splitPoint = p2 - v3 / 2;
						// New triangle 1
						AddTriangle(&tmpVertices, p2, splitPoint, p1);

						// New triangle 2
						AddTriangle(&tmpVertices, p3, splitPoint, p1);
					}
				}
				else{
					if (sv2.squaredNorm() > sv3.squaredNorm()){
						// v2 longest
						splitPoint = p1 - v2 / 2;
						//Triangle 1
						AddTriangle(&tmpVertices, p1, splitPoint, p2);
						//Triangle 2
						AddTriangle(&tmpVertices, p2, splitPoint, p3);

					}
					else{
						// v3 longest
						splitPoint = p2 - v3 / 2;
						// New triangle 1
						AddTriangle(&tmpVertices, p2, splitPoint, p1);

						// New triangle 2
						AddTriangle(&tmpVertices, p3, splitPoint, p1);
					}
				}
				// Add normals for both triangles
				for (int k = 0; k < 2; k++){
					AddTriangle(&tmpNormals, n1, n2, n3);
				}
			}
			else{ // Add complete triangle to patchedVertices
				AddTriangle(&patchedVertices, p1, p2, p3);

				// Add normals
				AddTriangle(&patchedNormals, n1, n2, n3);
			}
		}

		// If there are any triangles left to patch then add em to vertices / normals
		if (newVertices.size() > 0){
			newVertices = tmpVertices;
			newNormals = tmpNormals;
		}
		// Else we're done with patching
		else{
			patchingDone = true;
		}
	}
}



void Model::Update() {
    BaseComponent::Update();
}



void Model::Start() {
	BaseComponent::Start();

	SplitTriangles();

	UpdateVerticesAndNormals();

	LoadVBOAndVAO();

}

// inverterad skalning

void Model::UpdateVerticesAndNormals(){
	Eigen::Matrix4f mat4 = GetGameObject()->transform.GetMatrix();

	for (int i = 0; i < patchedVertices.size(); i += 3){
		Vector4f v = Vector4f(patchedVertices[i + 0], patchedVertices[i + 1], patchedVertices[i + 2], 1);
		v = mat4 * v;
		patchedVertices[i + 0] = v.x();
		patchedVertices[i + 1] = v.y();
		patchedVertices[i + 2] = v.z();
	}

	Eigen::Matrix3f mat3;
	mat3 = mat4.block<3, 3>(0, 0);

	mat3 = mat3.inverse().eval();
	mat3 = mat3.transpose().eval();
	for (int i = 0; i < patchedNormals.size(); i += 3){
		Vector3f v = Vector3f(patchedNormals[i + 0], patchedNormals[i + 1], patchedNormals[i + 2]);
		v = mat3 * v;
		v.normalize();
		patchedNormals[i + 0] = v.x();
		patchedNormals[i + 1] = v.y();
		patchedNormals[i + 2] = v.z();
	}
}


void Model::LoadVBOAndVAO(){
	unsigned int program = Context::Instance().program;
	if (vertices.size() > 0) {
		glGenVertexArrays(1, &vertexArrayObjID);
		glBindVertexArray(vertexArrayObjID);

		unsigned int vertexBufferObjID;
		glGenBuffers(1, &vertexBufferObjID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, patchedVertices.size() * sizeof(GLfloat), &patchedVertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

		unsigned int normalsBufferObjIDCube;
		glGenBuffers(1, &normalsBufferObjIDCube);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjIDCube);
		glBufferData(GL_ARRAY_BUFFER, patchedNormals.size() * sizeof(GLfloat), &patchedNormals[0], GL_STATIC_DRAW);

		glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));


		/*
		unsigned int colorBufferObjID;
		glGenBuffers(1, &colorBufferObjID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, "uni_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "uni_Color"));*/


	}
	else {
		std::cerr << " Cube not loaded!" << std::endl;
	}
}

void Model::Render() {
    BaseComponent::Render();

	// Draw stuff or something
	glBindVertexArray(vertexArrayObjID);	// Select VAO

	glUseProgram(Context::Instance().program);

	// Calculate transform and send to shader

	/*Eigen::Matrix4f matrix = Context::Instance().camera->projectionMatrix *
							Context::Instance().camera->worldToViewMatrix *
							 GetGameObject()->transform.GetMatrix();*/

	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "projectionMatrix"), 1, GL_FALSE, Context::Instance().camera->projectionMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "worldToViewMatrix"), 1, GL_FALSE, Context::Instance().camera->worldToViewMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "transform"), 1, GL_FALSE, GetTransform()->GetMatrix().data());
	glProgramUniform4fv(Context::Instance().program, glGetUniformLocation(Context::Instance().program, "uni_Color"), 1, colors);

	glDrawArrays(GL_TRIANGLES, 0, patchedVertices.size() / 3); // use GL_LINE_STRIP to kind of see the grid, not totaly correct
}

void Model::SetColor(GLfloat c1, GLfloat c2, GLfloat c3, GLfloat c4) {
	colors[0] = c1;
	colors[1] = c2;
	colors[2] = c3;
	colors[3] = c4;
}
