#include "scene.h"
#include "obj/model.h"
#include "obj/load.h"
#include "obj/draw.h"

#include <GL/gl.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 1024

//_____________________DRAW_________________________________________________________________

void draw_triangles(const Model* model)
{
    int i, k;
    int vertex_index, texture_index, normal_index;
    float x, y, z, u, v;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {
        for (k = 0; k < 3; ++k) {

            normal_index = model->triangles[i].points[k].normal_index;
            x = model->normals[normal_index].x;
            y = model->normals[normal_index].y;
            z = model->normals[normal_index].z;
            glNormal3f(x, y, z);

            texture_index = model->triangles[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1.0 - v);

            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}


void draw_model(const Model* model)
{
    draw_triangles(model);
}



void wall_front(GLuint texture_id)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Textúra ismétlés engedélyezése
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Textúra szűrés
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBegin(GL_QUADS);

    // 🔹 Most minden vertexhez tartozik texcoord is!

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void wall_sides(GLuint texture_id)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Textúra ismétlés engedélyezése
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Textúra szűrés
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBegin(GL_QUADS);

    // 🔹 Most minden vertexhez tartozik texcoord is!

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}


void draw_floor(GLuint texture_id)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 🔧 Szebb textúraszűrés
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);

    glTexCoord2f(-1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glTexCoord2f(1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glTexCoord2f(-1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}



//_____________________DRAW_________________________________________________________________



void allocate_model(Model* model)
{
    model->vertices = (Vertex*)malloc((model->n_vertices + 1) * sizeof(Vertex));
    model->texture_vertices = (TextureVertex*)malloc((model->n_texture_vertices + 1) * sizeof(TextureVertex));
    model->normals = (Vertex*)malloc((model->n_normals + 1) * sizeof(Vertex));
    model->triangles = (Triangle*)malloc(model->n_triangles * sizeof(Triangle));

    if (model->vertices == NULL || model->texture_vertices == NULL ||
        model->normals == NULL || model->triangles == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed in allocate_model()\n");
        exit(1);
    }
}

void init_model(Model* model)
{
    model->n_vertices = 0;
    model->n_texture_vertices = 0;
    model->n_normals = 0;
    model->n_triangles = 0;
}

int read_vertex(Vertex* vertex, const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        vertex->x = atof(&text[i]);
    }
    else {
        printf("The x value of vertex is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        vertex->y = atof(&text[i]);
    }
    else {
        printf("The y value of vertex is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        vertex->z = atof(&text[i]);
    }
    else {
        printf("The z value of vertex is missing!\n");
        return FALSE;
    }
    return TRUE;
}

int read_texture_vertex(TextureVertex* texture_vertex, const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        texture_vertex->u = atof(&text[i]);
    }
    else {
        printf("The u value of texture vertex is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        texture_vertex->v = atof(&text[i]);
    }
    else {
        printf("The v value of texture vertex is missing!\n");
        return FALSE;
    }
    return TRUE;
}

int read_normal(Vertex* normal, const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        normal->x = atof(&text[i]);
    }
    else {
        printf("The x value of normal vector is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        normal->y = atof(&text[i]);
    }
    else {
        printf("The y value of normal vector is missing!\n");
        return FALSE;
    }
    while (text[i] != 0 && text[i] != ' ') {
        ++i;
    }
    while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
        ++i;
    }
    if (text[i] != 0) {
        normal->z = atof(&text[i]);
    }
    else {
        printf("The z value of normal vector is missing!\n");
        return FALSE;
    }
    return TRUE;
}

int read_triangle(Triangle* triangle, const char* text)
{
    int point_index;
    int i;

    i = 0;
    for (point_index = 0; point_index < 3; ++point_index) {
        while (text[i] != 0 && is_numeric(text[i]) == FALSE) {
            ++i;
        }
        if (text[i] != 0) {
            triangle->points[point_index].vertex_index = atoi(&text[i]);
        }
        else {
            printf("The vertex index of the %d. points is missing!\n", point_index + 1);
            return FALSE;
        }
        while (text[i] != 0 && text[i] != '/') {
            ++i;
        }
        ++i;
        if (text[i] != 0) {
            triangle->points[point_index].texture_index = atoi(&text[i]);
        }
        else {
            printf("The texture index of the %d. points is missing!\n", point_index + 1);
            return FALSE;
        }
        while (text[i] != 0 && text[i] != '/') {
            ++i;
        }
        ++i;
        if (text[i] != 0) {
            triangle->points[point_index].normal_index = atoi(&text[i]);
        }
        else {
            printf("The normal index of the %d. points is missing!\n", point_index + 1);
            return FALSE;
        }
        while (text[i] != 0 && text[i] != ' ') {
            ++i;
        }
    }
    return TRUE;
}


int read_elements(Model* model, FILE* file)
{
    char line[LINE_BUFFER_SIZE];
    int vertex_index;
    int texture_index;
    int normal_index;
    int triangle_index;
    int success;

    allocate_model(model);
    vertex_index = 1;
    texture_index = 1;
    normal_index = 1;
    triangle_index = 0;
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        switch (calc_element_type(line)) {
        case NONE:
            break;
        case VERTEX:
            success = read_vertex(&(model->vertices[vertex_index]), line);
            if (success == FALSE) {
                printf("Unable to read vertex data!\n");
                return FALSE;
            }
            ++vertex_index;
            break;
        case TEXTURE_VERTEX:
            success = read_texture_vertex(&(model->texture_vertices[texture_index]), line);
            if (success == FALSE) {
                printf("Unable to read texture vertex data!\n");
                return FALSE;
            }
            ++texture_index;
            break;
        case NORMAL:
            success = read_normal(&(model->normals[normal_index]), line);
            if (success == FALSE) {
                printf("Unable to read normal vector data!\n");
                return FALSE;
            }
            ++normal_index;
            break;
        case FACE:
            success = read_triangle(&(model->triangles[triangle_index]), line);
            if (success == FALSE) {
                printf("Unable to read triangle face data!\n");
                return FALSE;
            }
            ++triangle_index;
            break;
        }
    }
    return TRUE;
}



ElementType calc_element_type(const char* text)
{
    int i;

    i = 0;
    while (text[i] != 0) {
        if (text[i] == 'v') {
            if (text[i + 1] == 't') {
                return TEXTURE_VERTEX;
            }
            else if (text[i + 1] == 'n') {
                return NORMAL;
            }
            else {
                return VERTEX;
            }
        }
        else if (text[i] == 'f') {
            return FACE;
        }
        else if (text[i] != ' ' && text[i] != '\t') {
            return NONE;
        }
        ++i;
    }
    return NONE;
}

void count_elements(Model* model, FILE* file)
{
    char line[LINE_BUFFER_SIZE];

    init_model(model);
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        switch (calc_element_type(line)) {
        case NONE:
            break;
        case VERTEX:
            ++model->n_vertices;
            break;
        case TEXTURE_VERTEX:
            ++model->n_texture_vertices;
            break;
        case NORMAL:
            ++model->n_normals;
            break;
        case FACE:
            ++model->n_triangles;
            break;
        }
    }
}

int load_model(Model* model, const char* filename)
{
    FILE* obj_file;
    int success;

    obj_file = fopen(filename, "r");
    printf("Load model '%s' ...\n", filename);
    if (obj_file == NULL) {
        printf("ERROR: Unable to open '%s' file!\n", filename);
        return FALSE;
    }
    printf("Count the elements ...\n");
    count_elements(model, obj_file);
    printf("Allocate memory for model ...\n");
    allocate_model(model);
    printf("Read model data ...\n");
    fseek(obj_file, 0, SEEK_SET);
    success = read_elements(model, obj_file);
    if (success == FALSE) {
        printf("ERROR: Unable to read the model data!\n");
        return FALSE;
    }
    return TRUE;
}

int is_numeric(char c)
{
    if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void init_scene(Scene* scene)
{
    load_model(&(scene->cube), "assets/models/cube.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");
    scene->floor_id = load_texture("assets/textures/padlo.jpg");


    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    glBindTexture(GL_TEXTURE_2D, scene->floor_id);


    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    

    float ambient_light1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float diffuse_light1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular_light1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float position1[] = { 10.0f, 0.0f, 0.0f, 1.0f }; 

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    glDisable(GL_LIGHTING); //tesztekre
    draw_origin();
    set_material(&(scene->material));
    set_lighting();
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    glEnable(GL_TEXTURE_2D);
    
    //Walls
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    wall_front(scene->floor_id);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    wall_front(scene->floor_id);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    wall_sides(scene->floor_id);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    wall_sides(scene->floor_id);
    glPopMatrix();

    //Pad ló - tető
    glPushMatrix();
    draw_floor(scene->floor_id);   
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.0f);
    draw_floor(scene->floor_id);
    glPopMatrix();



    glDisable(GL_TEXTURE_2D);
}


void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
