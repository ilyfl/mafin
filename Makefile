FLAGS += -Wall -Wextra -pedantic -std=c99 -fcommon 
OBJECTS= obj/stream.o obj/misc.o obj/fin.o obj/argums.o
CC = cc
stream.o :
	mkdir -p obj
	$(CC) -c src/stream.c -o obj/stream.o $(FLAGS) 
misc.o : stream.o
	$(CC) -c src/misc.c -o obj/misc.o $(FLAGS)
argums.o : misc.o
	$(CC) -c src/argums.c -o obj/argums.o $(FLAGS)
fin.o : argums.o 
	$(CC) -c src/fin.c -o obj/fin.o $(FLAGS)


		
install : fin.o 
	mkdir -p bin
	$(CC) -o bin/fin $(OBJECTS) $(FLAGS) -lm

ifeq (,$(wildcard $(HOME)/.config/mafin/config))
		mkdir -p $(HOME)/.config/mafin/
		touch $(HOME)/.config/mafin/config 
		echo "# This is configuration file for mafin program" > $(HOME)/.config/mafin/config 
		echo "# It has following syntax:" >> $(HOME)/.config/mafin/config
		echo "# <variable> = <value>">> $(HOME)/.config/mafin/config
		echo "">> $(HOME)/.config/mafin/config
		echo "dbpath = $(HOME)/mafin/finance.db">> $(HOME)/.config/mafin/config
		echo "resources = Cash, Card">> $(HOME)/.config/mafin/config
		echo "expenses = Food,Eating Out,Entertainment, Transport, Bills, Clothes, Health, Phone, Toiletry, Other" >> $(HOME)/.config/mafin/config
		echo "incomes = Salary,Random" >> $(HOME)/.config/mafin/config
		echo "currencies = USD, EUR" >> $(HOME)/.config/mafin/config
endif


debug : fin.o  argums.o misc.o stream.o
	$(CC) -o bin/fin $(OBJECTS) $(FLAGS)

