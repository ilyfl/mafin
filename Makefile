FLAGS= -Wall -Wextra -pedantic -std=c99 -lm
OBJECTS= obj/stream.o obj/getfuncs.o obj/fin.o obj/mysql.o obj/argums.o
stream.o : src/stream.c src/stream.h src/fin.h getfuncs.o
	gcc -c src/stream.c -o obj/stream.o $(FLAGS) 
getfuncs.o : src/getfuncs.c src/fin.h argums.o
	gcc -c src/getfuncs.c -o obj/getfuncs.o $(FLAGS)
argums.o : src/argums.c src/fin.h src/stream.h mysql.o
	gcc -c src/argums.c -o obj/argums.o $(FLAGS)
mysql.o : src/mysql.c src/fin.h fin.o
	gcc -c src/mysql.c -o obj/mysql.o `mysql_config --cflags --libs` $(FLAGS)
fin.o : src/fin.c src/fin.h src/stream.h src/misc.h
	gcc -c src/fin.c -o obj/fin.o
install : stream.o
	gcc -o bin/fin $(OBJECTS) `mysql_config --cflags --libs` $(FLAGS)
FLAGS =-Wall -Wextra -pedantic -std=c99 -lm -g
debug : install
