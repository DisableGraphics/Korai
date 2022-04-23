all:
	#----------------------------------------------------
	#	BUILDING KORAI-NODOWNLOAD
	#----------------------------------------------------
	mkdir -p build & g++ src/main.cpp `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` -larchive -O3 -std=c++17 -o build/korai-nodownload
	#----------------------------------------------------
	#	BUILDING KORAI
	#----------------------------------------------------
	mkdir -p build & g++ src/main.cpp `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` `pkg-config vte-2.91 --libs --cflags` -DDOWNLOAD -larchive -O3 -std=c++17 -o build/korai 
