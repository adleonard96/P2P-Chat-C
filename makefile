all:server launcher

server: server.c
		gcc  server.c -o Server -lpthread 

launcher: Launcher.c
		gcc Launcher.c -o launcher -lpthread
		
clean:
	rm -f Server Client launcher
