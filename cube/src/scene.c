#include "scene.h"
#include "obj/model.h"
#include "obj/load.h"
#include "draw.c"

#include <GL/gl.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 1024
float light_intensity = 1.0f; 
bool is_rotating = false;
static float rotation_angle = 0.0f;


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
    load_model(&(scene->stand), "assets/models/WORLD.obj");
    load_model(&(scene->round), "assets/models/golo.obj");
    scene->texture_id = load_texture("assets/textures/fold.jpg");
    scene->floor_id = load_texture("assets/textures/fal.jpg");
    scene->wood_id = load_texture("assets/textures/padlo.jpg");
    scene->help_id = load_texture("assets/textures/help.png");
    scene->need_help=false;

    


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
    float ambient_light[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
    float diffuse_light[]  = { light_intensity, light_intensity, light_intensity, 1.0f };
    float specular_light[] = { light_intensity * 0.2f, light_intensity * 0.2f, light_intensity * 0.2f, 1.0f };

    float position[]  = { -2.0f, -2.0f, 0.4f, 1.0f };
    float position2[] = {  1.0f,  1.0f, 0.4f, 1.0f };
    float position3[] = {  1.0f, -2.0f, 0.4f, 1.0f };
    float position4[] = { -2.0f,  1.0f, 0.4f, 1.0f };

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT1, GL_POSITION, position2);

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT2, GL_POSITION, position3);

    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT3, GL_POSITION, position4);
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
    if (is_rotating) {
        rotation_angle += 0.05f; 
        if (rotation_angle > 360.0f) {
            rotation_angle -= 360.0f;
        }
    }
}

void apply_shadow_matrix()
{
    GLfloat shadow_mat[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,  
        0, 0, 0, 0,
        0, 0, 0, 1
    };

    glMultMatrixf(shadow_mat);
}


void render_scene(const Scene* scene)
{
    //glDisable(GL_LIGHTING); //tesztekre
    //draw_origin();
    set_material(&(scene->material));
    set_lighting();
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glScalef(0.2f,0.2f,0.2f);
    glBindTexture(GL_TEXTURE_2D, scene->wood_id);
    draw_model(&(scene->stand));
    glPopMatrix();

    //shadow
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); 

    glTranslatef(0.0f, 0.0f, 0.01f);
    apply_shadow_matrix();

    glScalef(0.25f, 0.2f, 0.25f); 
    draw_model(&(scene->stand));

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.41f);
    glScalef(0.135f,0.135f,0.135f);
    glRotatef(rotation_angle, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    draw_model(&(scene->round));
    glPopMatrix();

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

    if (scene->need_help) {
        draw_help_overlay(scene->help_id);
    }    


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
