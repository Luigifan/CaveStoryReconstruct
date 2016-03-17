compile:
	clang++ -o gen/cavestory src/**.cpp -lSDL2
compile-osx:
	clang++ -o gen/cavestory src/**.cpp -L/usr/local/Cellar/sdl2/2.0.4/lib
run:
	gen/cavestory
