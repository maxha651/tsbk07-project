//
// Created by David on 2015-04-14.
//
// Component for model and shader

#include <Model.h>

static const char MODEL_REL_PATH[] = TSBK07_MODELS_PATH;
static const char SHADER_REL_PATH[] = TSBK07_SHADERS_PATH;

Model::Model(const std::string& jsonPath) :
    jsonLoader(jsonPath) {

    jsonLoader.AddDataField<std::string>("model", &model);
    //jsonLoader.AddDataField<std::string>("vertshader", &vertshader);
	//jsonLoader.AddDataField<std::string>("fragshader", &fragshader);
    jsonLoader.Read();

	init(model.c_str());
}

Model::Model(const char *model)
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


	// Load the VBO and VAO
	LoadVBOAndVAO();

}

Model::~Model()
{
    // JSONLoader will automatically update JSON file on destruct
}



void Model::LoadObject(const char* filename)
{

    std::vector<std::string*> coord; // Each line read from file
    std::vector<Vector3f> vertex;
	std::vector<Vector3f> normal;
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "Model file could not be opened" << std::endl;
        return;
    }
    char buf[256];
    // Read in every line to coord
    while (!in.eof())
    {
        in.getline(buf, 256);
        coord.push_back(new std::string(buf));
    }

    // Go through all elements of the coord and decide what kind of element it is
    for (size_t i = 0; i < coord.size(); i++) 
    {
        if (coord[i]->c_str()[0] == '#')   // Comment
            continue;      
        else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == ' ') // Vertice
        {
			GLfloat tmpx, tmpy, tmpz;
            sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz); // Read in the 3 float coordinate to tmpx,tmpy,tmpz
			vertex.push_back(Vector3f(tmpx, tmpy, tmpz));       // and then add it to the end of our vertex list
		
        }
        else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 'n') // Normal
        {
            float tmpx, tmpy, tmpz; 
            sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normal.push_back(Vector3f(tmpx, tmpy, tmpz));       // and then add it to the end of our vertex list

        }
        else if (coord[i]->c_str()[0] == 'f') // Face
        {
			
            if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3) // Check if triangle
            {
				unsigned int vertexIndex[3], normalIndex[3]; // uvIndex[3],
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);

				// Add vertices to final vertice list
				for (int n = 0; n < 3; n++){
					vertices.push_back(vertex[(vertexIndex[n] - 1)][0]);
					vertices.push_back(vertex[(vertexIndex[n] - 1)][1]);
					vertices.push_back(vertex[(vertexIndex[n] - 1)][2]);
					normals.push_back(normal[(normalIndex[n] - 1)][0]);
					normals.push_back(normal[(normalIndex[n] - 1)][1]);
					normals.push_back(normal[(normalIndex[n] - 1)][2]);
				}
            }
            else{ // else quad
				
				unsigned int vertexIndex[4], normalIndex[4]; // uvIndex[3],
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2], &vertexIndex[3], &normalIndex[3]);

				for (int n = 0; n < 4; n++){
					vertices.push_back(vertex[(vertexIndex[n] - 1)][0]);
					vertices.push_back(vertex[(vertexIndex[n] - 1)][1]);
					vertices.push_back(vertex[(vertexIndex[n] - 1)][2]);
					normals.push_back(normal[(normalIndex[n] - 1)][0]);
					normals.push_back(normal[(normalIndex[n] - 1)][1]);
					normals.push_back(normal[(normalIndex[n] - 1)][2]);
				}
            }
        } 
    }
}

void Model::Update() {
    BaseComponent::Update();

}



GLfloat triangle_colors[] = {
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,

	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,

	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,

	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,

	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
};

GLfloat t_colors[] = {
	1.0, 1.0, 0.0,
};

GLint attribute_v_color;
GLuint colorBufferObjID;

void Model::LoadVBOAndVAO(){
	unsigned int program = Context::Instance().program;
	if (vertices.size() > 0) {
		glGenVertexArrays(1, &vertexArrayObjID);
		glBindVertexArray(vertexArrayObjID);

		unsigned int vertexBufferObjID;
		glGenBuffers(1, &vertexBufferObjID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

		unsigned int normalsBufferObjIDCube;
		glGenBuffers(1, &normalsBufferObjIDCube);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjIDCube);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

		glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));


		color.push_back(1.0);
		color.push_back(1.0);
		color.push_back(1.0);

		unsigned int colorBufferObjID;
		glGenBuffers(1, &colorBufferObjID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);


		glVertexAttribPointer(glGetAttribLocation(program, "uni_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "uni_Color"));

	}
	else {
		std::cerr << " Cube not loaded!" << std::endl;
	}
}

void Model::Render() {
    BaseComponent::Render();
	glUseProgram(Context::Instance().program);
    // Draw stuff or something
	glBindVertexArray(vertexArrayObjID);	// Select VAO

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
}