# TreasureHunt

OS Treasure_hunt project

## Treasure Manager

To compile the program

```c
gcc -Wall -o treasure_manager treasure_manager.c
```

For addind new hunts/treasure

```c
./treasure_manager --add <huntId>
```

For listing all treasures from a hunt

```c
./treasure_manager --list <huntId>
```

For viewing a specific treasure from a hunt

```c
./treasure_manager --view <huntId> <treasureId>
```

For removing a specific treasure from a hunt

```c
./treasure_manager --remove <huntId> <treasureId>
```

For removing an entire hunt

```c
./treasure_manager --remove <huntId>
```

## Treasure hub

To compile the program

```c
gcc -Wall -o treasure_hub treasure_hub.c
```

To start the program

```c
./treasure_hub
```

Choose a command from the interactive interface<br />
- start_monitor<br />
- list_hunts<br />
- list_treasures<br />
- view_treasure<br />
- stop_monitor<br />
- exit
