#
# this makefile will compile and and all source
# found in the "MyApp" directory.  This represents a sample
# development directory structure and project
# 
# =======================================================
#                  MyApp Example
# =======================================================
# FINAL BINARY Target
./bin/Main : ./obj/Main.o ./obj/Network.o ./obj/Window.o
	cc  ./obj/Main.o ./obj/Network.o ./obj/Window.o -o ./bin/chat-client -lncurses -lpthread
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/Main.o : ./src/Main.c ./inc/clientHeader.h
	cc -c -g ./src/Main.c -o ./obj/Main.o

./obj/Network.o : ./src/Network.c ./inc/clientHeader.h
	cc -c -g ./src/Network.c -o ./obj/Network.o

./obj/Window.o : ./src/Window.c ./inc/clientHeader.h
	cc -c -g ./src/Window.c -o ./obj/Window.o

#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/chat-client

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


