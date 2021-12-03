build: Sketch
	./Sketch

Sketch: Sketch.c
	clang Sketch.c -l SDL2 -l SDL2_image -l SDL2_ttf -o Sketch