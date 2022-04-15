all:
	g++ src/main.cpp `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.1 --libs --cflags` -std=c++17 -o build/korai
