#include <math.h>
#include <string.h>
#include <stdbool.h>
#include"matrix.h"

#define STACK_SIZE 10

void print_matrix(const float matrix[3][3])
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void init_zero_matrix(float matrix[3][3])
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0f;
        }
    }
}


void init_identity_matrix(float matrix[3][3]) {
    init_zero_matrix(matrix);
    for (int i = 0; i < 3; ++i) {
        matrix[i][i] = 1.0f;
    }
}

void scalar_multiply_matrix(float matrix[3][3], float scalar) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] *= scalar;
}

void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3]) {
    float temp[3][3] = {0};

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                temp[i][j] += a[i][k] * b[k][j];

    memcpy(result, temp, sizeof(temp));
}

void transform_point(const float matrix[3][3], const float point[3], float result[3]) {
    for (int i = 0; i < 3; ++i) {
        result[i] = 0;
        for (int j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
}

void scale(float matrix[3][3], float sx, float sy) {
    float scale_mat[3][3];
    init_identity_matrix(scale_mat);
    scale_mat[0][0] = sx;
    scale_mat[1][1] = sy;

    multiply_matrices(scale_mat, matrix, matrix);
}

void shift(float matrix[3][3], float dx, float dy) {
    float shift_mat[3][3];
    init_identity_matrix(shift_mat);
    shift_mat[0][2] = dx;
    shift_mat[1][2] = dy;

    multiply_matrices(shift_mat, matrix, matrix);
}

void rotate(float matrix[3][3], float angle) {
    float rot_mat[3][3];
    init_identity_matrix(rot_mat);
    float rad = angle * M_PI / 180.0f;

    rot_mat[0][0] = cos(rad);
    rot_mat[0][1] = -sin(rad);
    rot_mat[1][0] = sin(rad);
    rot_mat[1][1] = cos(rad);

    multiply_matrices(rot_mat, matrix, matrix);
}

static float matrix_stack[STACK_SIZE][3][3];
static int stack_top = -1;

void push_matrix(const float matrix[3][3]) {
    if (stack_top < STACK_SIZE - 1) {
        ++stack_top;
        memcpy(matrix_stack[stack_top], matrix, sizeof(float) * 9);
    }
}

bool pop_matrix(float matrix[3][3]) {
    if (stack_top >= 0) {
        memcpy(matrix, matrix_stack[stack_top], sizeof(float) * 9);
        --stack_top;
        return true;
    }
    return false;
}
