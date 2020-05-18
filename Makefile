FLAGS= -Wall -Wextra -pedantic -std=c99 -lm
OBJECTS= obj/stream.o obj/misc.o obj/fin.o obj/mysql.o obj/argums.o
stream.o : src/stream.c src/stream.h src/fin.h src/misc.h
	gcc -c src/stream.c -o obj/stream.o $(FLAGS) 
misc.o : src/misc.c src/fin.h src/misc.h
	gcc -c src/misc.c -o obj/misc.o $(FLAGS)
argums.o : src/argums.c src/fin.h src/stream.h
	gcc -c src/argums.c -o obj/argums.o $(FLAGS)
mysql.o : src/mysql.c src/fin.h 
	gcc -c src/mysql.c -o obj/mysql.o `mysql_config --cflags --libs` $(FLAGS)
fin.o : src/fin.c src/fin.h src/stream.h src/misc.h
	gcc -c src/fin.c -o obj/fin.o
install : fin.o mysql.o argums.o misc.o stream.o
	gcc -o bin/fin $(OBJECTS) `mysql_config --cflags --libs` $(FLAGS)
FLAGS=  -g -Wall -Wextra -pedantic -std=c99 -lm
debug : fin.o mysql.o argums.o misc.o stream.o
	gcc -o bin/fin $(OBJECTS) `mysql_config --cflags --libs` $(FLAGS) 

