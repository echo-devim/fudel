all:
	g++ -std=c++17 -O3 -o fudel fudel.cpp gui.cpp -lsqlite3 -lstdc++fs `pkg-config gtkmm-3.0 --cflags --libs` `pkg-config --cflags --libs libnotify`
