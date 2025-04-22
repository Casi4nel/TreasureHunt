all: treasure_manager treasure_hub

treasure_manager: treasure_manager.c
	gcc -Wall -o treasure_manager treasure_manager.c

treasure_hub: treasure_hub.c
	gcc -Wall -o treasure_hub treasure_hub.c

clean:
	rm -f treasure_manager treasure_hub
