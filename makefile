all:
	g++ -std=c++0x -static-libstdc++ -g -o game.exe character.cpp priest.cpp human.cpp warrior.cpp undead.cpp environment.cpp graveyard.cpp forest.cpp field.cpp sea.cpp item.cpp container.cpp main.cpp direction.cpp game.cpp
