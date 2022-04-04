#
# this makefile will compile and and all source
# found in the "MyApp" directory.  This represents a sample
# development directory structure and project
# 
# =======================================================
#                  MyApp Example
# =======================================================
# FINAL BINARY Target
./bin/Main : ./obj/Main.o ./obj/LinkedList.o ./obj/ThreadWorkers.o ./obj/Utility.o 
	cc ./obj/Main.o ./obj/LinkedList.o ./obj/ThreadWorkers.o ./obj/Utility.o  -o ./bin/chat-server -lncurses -lpthread
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/Main.o : ./src/Main.c ./inc/serverHeader.h
	cc -c -g ./src/Main.c -o ./obj/Main.o

./obj/LinkedList.o : ./src/LinkedList.c ./inc/serverHeader.h
	cc -c -g ./src/LinkedList.c -o ./obj/LinkedList.o

./obj/ThreadWorkers.o : ./src/ThreadWorkers.c ./inc/serverHeader.h
	cc -c -g ./src/ThreadWorkers.c -o ./obj/ThreadWorkers.o

./obj/Utility.o : ./src/Utility.c ./inc/serverHeader.h
	cc -c -g ./src/Utility.c -o ./obj/Utility.o


#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/chat-server

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


