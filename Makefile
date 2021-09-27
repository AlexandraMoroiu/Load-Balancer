build:
	gcc -std=c99 -Wall -Wextra circular_doubly_linked_list.c circular_doubly_linked_list.h -c
	gcc -std=c99 -Wall -Wextra hashring.c hashring.h -c
	gcc -std=c99 -Wall -Wextra hashtable.c hashtable.h -c
	gcc -std=c99 -Wall -Wextra linked_list.c linked_list.h -c
	gcc -std=c99 -Wall -Wextra load_balancer.c load_balancer.h -c
	gcc -std=c99 -Wall -Wextra main.c -c
	gcc -std=c99 -Wall -Wextra server.c server.h -c
	gcc circular_doubly_linked_list.o hashring.o hashtable.o linked_list.o load_balancer.o main.o server.o -o tema2

pack:
	zip -FSr 315CA_AlexandraMoroiu_Tema2.zip README Makefile *.c *.h

clean:
	rm -f tema2 *.o *.gch
