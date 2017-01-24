#include <stdlib.h>
#include <stdio.h>
#include <libdkcst/dkcst_source.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
	
	printf("Initializing DkCst\n");
    DkCst_init();
	printf("Initialized DkCst\n");

	DkCstApp* app;
	
    DkCst_create_app(&app);
	
	printf("Terminating DkCst\n");
    DkCst_terminate();
	printf("Terminated DkCst\n");
	
	return 0; 
	
}
