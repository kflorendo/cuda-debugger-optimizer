gui: src/gui.h src/gui.cpp src/main.cpp
	g++ src/gui.cpp src/main.cpp -o gui `pkg-config --cflags --libs gtkmm-4.0` -std=c++17