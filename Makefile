gcc src/store.c -c -o obj/store.o -Wall -Wextra -std=c99
gcc src/getfuncs.c -c -o obj/getfuncs.o -Wall -Wextra -std=c99 
gcc src/mysql.c -c -o obj/mysql.o -Wall -Wextra -std=c99 `mysql_config --cflags --libs` 
gcc src/fin.c -c -o obj/fin.o -Wall -Wextra -std=c99 
gcc -o bin/fin obj/*.o -Wall -Wextra -std=c99 `mysql_config --cflags --libs` 
