run: lena2048
	./lena2048

lena2048: lena2048.cpp
	g++ -o lena2048 lena2048.cpp -lncurses
