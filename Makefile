all:
	gcc -Iinclude/ -Iinclude/obj/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm -o cube.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ -Iinclude/obj/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lSDL2 -lSDL2_image -lGL -lm -o cube -Wall -Wextra -Wpedantic

clean:
	rm -f cube cube.exe
