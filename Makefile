roguelike: main.o dungeon.o queue.o room.o npc.o player.o file_reader.o roll.o monster.o item_template.o item.o
	g++ -ggdb main.o room.o queue.o dungeon.o npc.o player.o roll.o file_reader.o monster.o item_template.o item.o -o roguelike -lm -lncurses

main.o: main.h main.cpp
	g++ -ggdb main.cpp -c -Wall -lm -lncurses

room.o: room.h room.c
	gcc -ggdb room.c -c -Wall -lm

dungeon.o: dungeon.h dungeon.c
	gcc -ggdb dungeon.c -c -Wall -lm -lncurses

npc.o: npc.h npc.c middle.h
	gcc -ggdb npc.c -c -Wall -lm

player.o: player.h player.c middle.h
	gcc -ggdb player.c -c -Wall -lm

queue.o: queue.h queue.c
	gcc -ggdb queue.c -c -Wall -lm

file_reader.o: file_reader.h file_reader.cpp
	g++ -ggdb file_reader.cpp -c -Wall

roll.o: roll.h roll.cpp
	g++ -ggdb roll.cpp -c -Wall

monster.o: monster.h monster.cpp middle.h
	g++ -ggdb monster.cpp -c -Wall

item_template.o: item_template.h item_template.cpp
	g++ -ggdb item_template.cpp -c -Wall

item.o: item.h item.cpp
	g++ -ggdb item.cpp -c -Wall