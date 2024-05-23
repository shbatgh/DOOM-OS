#include <stdio.h>

#include <kernel/tty.h>


void kernel_main(void) {
	terminal_initialize();
	for (int i = 0; i < 141; i++) {
		printf("%d\n", i);
	}
	printf("Hello, kernel World!\n");
	
	//printf("deezaaeds\n");
}
