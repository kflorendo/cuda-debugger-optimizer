gui: src/gui.cpp
	g++ src/gui.cpp -o gui `pkg-config --cflags --libs gtkmm-3.0` -std=c++17