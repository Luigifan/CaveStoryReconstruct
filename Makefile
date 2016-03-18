compile:
	clang++ -std=c++11 -stdlib=libc++ -o gen/cavestory src/**.cpp -lSDL2 -lSDL2_image
compile-osx:
	clang++ -std=c++11 -o gen/cavestory src/**.cpp -L/usr/local/Cellar/sdl2/2.0.4/lib
run:
	gen/cavestory
