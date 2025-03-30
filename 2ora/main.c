#include "matrix.c"
#include <stdio.h>

int main() {
    float m[3][3];
    float p[3] = {1.0f, 2.0f, 1.0f};
    float result[3];

    init_identity_matrix(m);
    shift(m, 3, 4);
    scale(m, 2, 2);
    rotate(m, 45);
    print_matrix(m);

    transform_point(m, p, result);
    printf("Eredeti pont: (%f, %f)\n", p[0], p[1]);
    printf("Transzformált pont: (%f, %f)\n", result[0], result[1]);

    // Verem működés:
    push_matrix(m);
    shift(m, -2, 0);
    print_matrix(m);
    pop_matrix(m);
    printf("Visszavont mátrix:\n");
    print_matrix(m);

    return 0;
}
