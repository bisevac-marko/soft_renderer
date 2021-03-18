#include "math.h"

struct Face
{
    vec3i vertex_index;
    vec3i text_coord;
    vec3i normal;
};

struct ModelInfo
{
    i32 vertex_count;
    i32 face_count;
    i32 texture_vertex_count;
};

struct Model
{
    vec3* vertices;
    Face* faces;
    vec3* texture_uvs;

    ModelInfo model_info;
};

internal ModelInfo
get_model_info(char* file)
{
    ModelInfo out = {};
    char line[512];
    FILE* fp = fopen(file, "r");

    if (file)
    {
        while(fgets(line, 512, fp)) 
        {
            if (line[0] == 'f' && line[1] == ' ')
            {
                out.face_count++;
            }
            else if (line[0] == 'v' && line[1] == 't')
            {
                out.texture_vertex_count++;
            }
            else if (line[0] == 'v' && line[1] == ' ')
            {
                out.vertex_count++;
            }
        }
    }
    fclose(fp);

    return out;
}

internal vec3
get_vertex_from_line(char line[512])
{
    vec3 out = {};
    float data[3];
    char number_str[50];
    int vertex_axis = 0;

    for (int i = 0; line[i] != '\0'; ++i)
    {
        int sign = 1;

        if (line[i] == '-')
        {
            sign = -1;
            i++;
        }
        if (isdigit(line[i]))
        {
            int number_str_index = 0;
            while(isdigit(line[i]) || line[i] == '.')
            {
                number_str[number_str_index] = line[i];
                number_str_index++;
                i++;
            }
            number_str[number_str_index] = '\0';
            data[vertex_axis] = (float)(atof(number_str) * sign);
            vertex_axis++;
            number_str_index = 0;
        }
    }

    out.x = data[0];
    out.y = data[1];
    out.z = data[2];

    return out;
}

internal Face
get_face_from_line(char line[512])
{
    Face out = {};
    int member_index = 0;

    for (int i = 0; line[i] != '\0'; ++i)
    {
        if (isdigit(line[i]))
        {
            char number_str[64];
            int data[3];

            for (int j = 0; j < 3; ++j)
            {
                int number_str_index = 0;
                while(isdigit(line[i]))
                {
                    number_str[number_str_index] = line[i];
                    number_str_index++;
                    i++;
                }
                number_str[number_str_index] = '\0';
                data[j] = (atoi(number_str));
                if (j != 2)
                {
                    i++;
                }
            }
            out.vertex_index[member_index] = data[0];
            out.text_coord[member_index] = data[1];
            out.normal[member_index]= data[2];

            member_index++;
        }
    }

    return out;
}
internal vec3
get_texture_vert_from_line(char line[512])
{
    vec3 out = {};
    float data[3];
    char number_str[50];
    int vertex_axis = 0;

    for (int i = 0; line[i] != '\0'; ++i)
    {
        int sign = 1;

        if (line[i] == '-')
        {
            sign = -1;
            i++;
        }
        if (isdigit(line[i]))
        {
            int number_str_index = 0;
            while(isdigit(line[i]) || line[i] == '.')
            {
                number_str[number_str_index] = line[i];
                number_str_index++;
                i++;
            }
            number_str[number_str_index] = '\0';
            data[vertex_axis] = (float)(atof(number_str) * sign);
            vertex_axis++;
            number_str_index = 0;
        }
    }

    out.x = data[0];
    out.y = data[1];
    out.z = data[2];

    return out;
}


internal Model
load_model(char* path)
{
    Model out = {};
    out.model_info = get_model_info(path);

    char line[512];
    FILE* file = fopen(path, "r");

    out.vertices = (vec3*)malloc(sizeof(vec3) * out.model_info.vertex_count);
    out.faces = (Face*)malloc(sizeof(Face) * out.model_info.face_count);
    out.texture_uvs = (vec3*)malloc(sizeof(vec3) * out.model_info.texture_vertex_count);

    vec3* vertices = out.vertices;
    Face* faces = out.faces;
    vec3* texture_uvs = out.texture_uvs;

    if (file)
    {
        while(fgets(line, 512, file)) 
        {
            if (line[0] == 'v' && line[1] == ' ')
            {
                *vertices = get_vertex_from_line(line);
                vertices++;
            }
            else if (line[0] == 'f' && line[1] == ' ')
            {
                *faces = get_face_from_line(line);
                faces++;
            }
            else if (line[0] == 'v' && line[1] == 't')
            {
                *texture_uvs = get_texture_vert_from_line(line);
                texture_uvs++;
            }
        }
    }
    else
    {
        DEBUG_PRINT("Failed to load model: %s.\n", path);
    }

    return out;
}

inline internal vec3
vertex_from_face(Face face, vec3* vertices, int index)
{
    return vertices[face.vertex_index[index]-1];
}

inline internal vec3
texture_uv_from_face(Face face, vec3* tex_vertices, int index)
{
    return tex_vertices[face.text_coord[index]-1];
}