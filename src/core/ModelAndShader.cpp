//
// Created by David on 2015-04-14.
//
// LoadObject function from https://www.youtube.com/watch?v=849hXuOv0i8

#include <ModelAndShader.h>

static const std::string MODEL_REL_PATH = TSBK07_MODELS_PATH;
static const std::string SHADER_REL_PATH = TSBK07_SHADERS_PATH;

ModelAndShader::ModelAndShader(const char *jsonPath) :
    jsonLoader(std::string(jsonPath)) {
    jsonLoader.addDataField<std::string>(&model);
    jsonLoader.addDataField<std::string>(&shader);
    jsonLoader.read();

    init(model.c_str(), shader.c_str());
}

ModelAndShader::ModelAndShader(const char *model, const char *shader)
{
    init(model, shader);
}

void ModelAndShader::init(const char *model, const char *shader) {
    std::string _model(MODEL_REL_PATH);
    std::string vertShader(SHADER_REL_PATH);
    std::string fragShader(SHADER_REL_PATH);

    // Get full object file path
    _model.append("/");
    _model.append(model);
    _model.append(".obj");

    // Load the model.
    id = LoadObject(_model.c_str());

    // Get full shader paths
    vertShader.append("/");
    fragShader.append("/");
    vertShader.append(shader);
    fragShader.append(shader);
    // TODO: Either use this convention or something else 
    // e.g. keep separate entries in JSON
    vertShader.append(".vert");
    fragShader.append(".frag");

    // Load the shader.
}

ModelAndShader::~ModelAndShader()
{
    // JSONLoader will automatically update JSON file on destruct
}


struct ModelAndShader::coordinate{
    float x, y, z;
    coordinate(float a, float b, float c) : x(a), y(b), z(c) {};
};


struct ModelAndShader::face {
    int facenum;
    bool quad; // True if using quads instead of triangles
    int faces[4];
    face(int facen, int f1, int f2, int f3) : facenum(facen){ // Triangle
        faces[0] = f1;
        faces[1] = f2;
        faces[2] = f3;
        quad = false;
    }
    face(int facen, int f1, int f2, int f3, int f4) : facenum(facen){ // Quad
        faces[0] = f1;
        faces[1] = f2;
        faces[2] = f3;
        faces[3] = f4;
        quad = true;
    }
};

int ModelAndShader::LoadObject(const char* filename)
{

	std::vector<std::string*> coord;
	std::vector<coordinate*> vertex;
	std::vector<face*> faces;
	std::vector<coordinate*> normals;
	std::ifstream in(filename);
	if (!in.is_open())
	{
		std::cout << "Model file could not be opened" << std::endl;
		return -1;
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
			float tmpx, tmpy, tmpz;
			sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz); // Read in the 3 float coordinate to tmpx,tmpy,tmpz
			vertex.push_back(new coordinate(tmpx, tmpy, tmpz));       // and then add it to the end of our vertex list
		}
		else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 'n') // Normal
		{
			float tmpx, tmpy, tmpz; 
			sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(new coordinate(tmpx, tmpy, tmpz));
		}
		else if (coord[i]->c_str()[0] == 'f') // Face
		{
			int a, b, c, d, e;
			if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3) // Check if triangle
			{
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
				faces.push_back(new face(b, a, c, d));     // Read in and add to the end of the face list
			}
			else{ // else quad
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
				faces.push_back(new face(b, a, c, d, e));
			}
		}
	}


	int num; // The id for the list
	num = glGenLists(1); // Generate a uniqe id
	glNewList(num, GL_COMPILE); // and create it
	for (size_t i = 0; i<faces.size(); i++)
	{
		if (faces[i]->quad) // If it's a quad draw a quad
		{
			glBegin(GL_QUADS);
			// Basically all I do here, is use the facenum (so the number of the face) as an index for the normal, so the 1st normal owe to the first face
			// I subtract 1 because the index start from 0 in C++
			glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z);
			// Draw the faces
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[3] - 1]->x, vertex[faces[i]->faces[3] - 1]->y, vertex[faces[i]->faces[3] - 1]->z);
			glEnd();
		}
		else{
			glBegin(GL_TRIANGLES);
			glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z);
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glEnd();
		}
	}
	glEndList();

	// Delete everything to avoid memory leaks
	for (size_t i = 0; i<coord.size(); i++)
		delete coord[i];
	for (size_t i = 0; i<faces.size(); i++)
		delete faces[i];
	for (size_t i = 0; i<normals.size(); i++)
		delete normals[i];
	for (size_t i = 0; i<vertex.size(); i++)
		delete vertex[i];
	return num; // Return the id
}
