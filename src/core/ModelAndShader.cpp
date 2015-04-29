//
// Created by David on 2015-04-14.
//
// LoadObject function from https://www.youtube.com/watch?v=849hXuOv0i8
// Also relevent: https://www.youtube.com/watch?v=dQw4w9WgXcQ

#include <ModelAndShader.h>

static const std::string MODEL_REL_PATH = TSBK07_MODELS_PATH;
static const std::string SHADER_REL_PATH = TSBK07_SHADERS_PATH;

ModelAndShader::ModelAndShader(const std::string& jsonPath) :
    jsonLoader(jsonPath) {

    jsonLoader.AddDataField<std::string>("model", &model);
    jsonLoader.AddDataField<std::string>("shader", &shader);
    jsonLoader.Read();

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
    LoadObject(_model.c_str());

    // Get full shader paths
    vertShader.append("/");
    fragShader.append("/");
    vertShader.append(shader);
    fragShader.append(shader);
    // TODO: Either use this convention or something else 
    // e.g. keep separate entries in JSON.
    vertShader.append(".vert");
    fragShader.append(".frag");

    // Load the shader.
}

ModelAndShader::~ModelAndShader()
{
    // JSONLoader will automatically update JSON file on destruct
}


void ModelAndShader::LoadObject(const char* filename)
{

    std::vector<std::string*> coord; // Each line read from file
    std::vector<GLfloat*> vertex;
	std::vector<GLfloat*> normals;
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
			vertex.push_back(&tmpx);       // and then add it to the end of our vertex list
			vertex.push_back(&tmpy);
			vertex.push_back(&tmpz);
        }
        else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 'n') // Normal
        {
            float tmpx, tmpy, tmpz; 
            sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(&tmpx);       // and then add it to the end of our vertex list
			normals.push_back(&tmpy);
			normals.push_back(&tmpz);
        }
        else if (coord[i]->c_str()[0] == 'f') // Face
        {
			
            if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3) // Check if triangle
            {
				unsigned int vertexIndex[3], normalIndex[3]; // uvIndex[3],
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                //faces.push_back(new face(b, a, c, d));     // Read in and add to the end of the face list

				// Add vertices to final vertice list
				out_vertices.push_back(vertex[(vertexIndex[0] - 1)]);
				out_vertices.push_back(vertex[(vertexIndex[1] - 1)]);
				out_vertices.push_back(vertex[(vertexIndex[2] - 1)]);

				out_normals.push_back(normals[(normalIndex[0] - 1)]);
				out_normals.push_back(normals[(normalIndex[1] - 1)]);
				out_normals.push_back(normals[(normalIndex[2] - 1)]);
            }
            else{ // else quad
				unsigned int vertexIndex[4], normalIndex[4]; // uvIndex[3],
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2], &vertexIndex[3], &normalIndex[3]);
                //faces.push_back(new face(b, a, c, d, e));

				out_vertices.push_back(vertex[(vertexIndex[0] - 1)]);
				out_vertices.push_back(vertex[(vertexIndex[1] - 1)]);
				out_vertices.push_back(vertex[(vertexIndex[2] - 1)]);
				out_vertices.push_back(vertex[(vertexIndex[3] - 1)]);

				out_normals.push_back(normals[(normalIndex[0] - 1)]);
				out_normals.push_back(normals[(normalIndex[1] - 1)]);
				out_normals.push_back(normals[(normalIndex[2] - 1)]);
				out_normals.push_back(normals[(normalIndex[3] - 1)]);
            }
        } 
    }

	/*GLuint *buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, &buf);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(GLfloat), &out_vertices[0], GL_STATIC_DRAW);*/


    // Delete everything to avoid memory leaks
   /* for (size_t i = 0; i<coord.size(); i++)
        delete coord[i];
    for (size_t i = 0; i<normals.size(); i++)
        delete normals[i];
    for (size_t i = 0; i<vertex.size(); i++)
        delete vertex[i];*/
}
