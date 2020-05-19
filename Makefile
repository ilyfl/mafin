FLAGS= -Wall -Wextra -pedantic -std=c99 -lm
OBJECTS= obj/stream.o obj/misc.o obj/fin.o obj/argums.o
stream.o : src/stream.c src/stream.h src/fin.h src/misc.h
	gcc -c src/stream.c -o obj/stream.o $(FLAGS) 
misc.o : src/misc.c src/fin.h src/misc.h
	gcc -c src/misc.c -o obj/misc.o $(FLAGS)
argums.o : src/argums.c src/fin.h src/stream.h
	gcc -c src/argums.c -o obj/argums.o $(FLAGS)
fin.o : src/fin.c src/fin.h src/stream.h src/misc.h
	gcc -c src/fin.c -o obj/fin.o


install : fin.o  argums.o misc.o stream.o
	gcc -o bin/fin $(OBJECTS) $(FLAGS)

ifeq (,$(wildcard /home/$(USER)/.config/mafin/config))
		mkdir -p /home/$(USER)/.config/mafin/
		touch /home/$(USER)/.config/mafin/config 
		echo "# This is configuration file for mafin program" > /home/$(USER)/.config/mafin/config 
		echo "# It has following syntax:" >> /home/$(USER)/.config/mafin/config
		echo "# <variable> = <value>">> /home/$(USER)/.config/mafin/config
		echo "">> /home/$(USER)/.config/mafin/config
		echo "dbpath = /home/$(USER)/mafin/finance.db">> /home/$(USER)/.config/mafin/config
		echo "resources = Cash, Card">> /home/$(USER)/.config/mafin/config
		echo "expenses = Food,Eating Out,Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other" >> /home/$(USER)/.config/mafin/config
		echo "incomes = Salary,Random" >> /home/$(USER)/.config/mafin/config
		echo "currencies = USD, EUR" >> /home/$(USER)/.config/mafin/config
endif

FLAGS=  -g -Wall -Wextra -pedantic -std=c99 -lm
debug : fin.o  argums.o misc.o stream.o
	gcc -o bin/fin $(OBJECTS) $(FLAGS) 

