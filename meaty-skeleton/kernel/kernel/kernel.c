#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	/*for (int i = 0; i < 140; i++) {
		printf("Hello, kernel World!");
	}*/
	printf("Hello, kernel World!");
	
	//printf("deezaaeds");
}
