all: treasure_manager treasure_hub calculate_score

treasure_manager: treasure_manager.c
	gcc -Wall -o treasure_manager treasure_manager.c

treasure_hub: treasure_hub.c
	gcc -Wall -o treasure_hub treasure_hub.c

calculate_score: calculate_score.c
	gcc -Wall -o calculate_score calculate_score.c

clean:
	rm -f treasure_manager treasure_hub calculate_score
