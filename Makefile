libgettimeofday.so: gettimeofday.c
	gcc -Wall -shared  -fPIC $^ -o $@ -ldl 
