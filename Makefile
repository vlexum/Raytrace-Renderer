all: raycast

raycast: raycaster.c main.c v3math.c ppmrw.c raycaster.h v3math.h ppmrw.h -lm
	gcc -std=c99 raycaster.c main.c v3math.c ppmrw.c -o raycast

clean:
	\rm -rf *.o raycast