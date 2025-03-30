#include <stdio.h>
#include <stdbool.h>

typedef struct {
    double a;
    double b;
    double c;
} Cuboid;

bool set_size(Cuboid *cuboid, double a, double b, double c) {
    if (a <= 0 || b <= 0 || c <= 0) {
        printf("Hiba: az élek hossza pozitív szám kell legyen!\n");
        return false;
    }
    cuboid->a = a;
    cuboid->b = b;
    cuboid->c = c;
    return true;
}

double calc_volume(Cuboid cuboid) {
    return cuboid.a * cuboid.b * cuboid.c;
}

double calc_surface(Cuboid cuboid) {
    return 2 * (cuboid.a * cuboid.b + cuboid.a * cuboid.c + cuboid.b * cuboid.c);
}

bool has_square_face(Cuboid cuboid) {
    return (cuboid.a == cuboid.b) || (cuboid.a == cuboid.c) || (cuboid.b == cuboid.c);
}

int main() {
    Cuboid box;
    
    if (!set_size(&box, 3.0, 4.0, 4.0)) {
        return 1; 
    }

    printf("Térfogat: %.2f\n", calc_volume(box));
    printf("Felszín: %.2f\n", calc_surface(box));

    if (has_square_face(box)) {
        printf("Van négyzet alakú lapja.\n");
    } else {
        printf("Nincs négyzet alakú lapja.\n");
    }

    return 0;
}
