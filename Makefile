build:
	gcc -Wall -std=c99 ./src/*.c -lglfw -lGL -lGLU -lGLEW -lm -o ./bin/raycaster
run:
	./bin/chip8 'roms/IBM Logo.ch8'
clean:
	rm ./bin/*
