gcc src/getfuncs.c -c -o obj/getfuncs.o -Wall -Wextra -std=c99 `mysql_config --cflags --libs` -g
gcc src/mysql.c -c -o obj/mysql.o -Wall -Wextra -std=c99 `mysql_config --cflags --libs` -g
gcc src/fin.c -c -o obj/fin.o -Wall -Wextra -std=c99 `mysql_config --cflags --libs` -g
gcc -o fin obj/*.o -Wall -Wextra -std=c99 `mysql_config --cflags --libs` -g
