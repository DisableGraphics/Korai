all: korai extensions

korai:
	#-----------------------------------------------------
	#	BUILDING KORAI
	#-----------------------------------------------------
	mkdir -p build & g++ src/main.cpp `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` `pkg-config vte-2.91 --libs --cflags` -larchive -pthread -O3 -std=c++17 -o build/korai

korai-nodownload: extensions
	#-----------------------------------------------------
	#	BUILDING KORAI-NODOWNLOAD
	#-----------------------------------------------------
	mkdir -p build & g++ src/main.cpp `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` -DNODOWNLOAD -larchive -pthread -O3 -std=c++17 -o build/korai-nodownload

extensions:
	#-----------------------------------------------------
	#	BUILDING EXTENSIONS
	#-----------------------------------------------------
	mkdir -p build
	mkdir -p build/korai-extensions
	g++ src/extensions/pos_save.cpp -shared -fPIC -o build/korai-extensions/savepos.so `pkg-config webkit2gtk-web-extension-4.0 --libs --cflags` -std=c++17