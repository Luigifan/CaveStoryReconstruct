compile:
	clang++ -std=c++11 -stdlib=libc++ -o gen/cavestory src/**.cpp -lSDL2
run:
	gen/cavestory
