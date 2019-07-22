
main: vm.o main.c
	gcc -o main vm.o main.c

vm.o: vm.h vm.c
	gcc -c vm.h vm.c

clean: 
	rm main vm.o