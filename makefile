all:
	g++ -std=c++0x -static-libstdc++ -g -o build/game.exe src/character.cpp src/priest.cpp src/human.cpp src/warrior.cpp src/undead.cpp src/environment.cpp src/graveyard.cpp src/forest.cpp src/field.cpp src/sea.cpp src/item.cpp src/container.cpp src/main.cpp src/direction.cpp src/game.cpp
