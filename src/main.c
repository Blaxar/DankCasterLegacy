#include <stdlib.h>
#include <stdio.h>
#include <libdkcst/dkcst_app.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
  
  printf("Initializing DkCst\n");
  DkCst_init();
  printf("Initialized DkCst\n");

  DkCstApp* app;
  
  if(!DkCst_rc_ok(DkCst_create_app(&app))) printf("Failed to create app.\n");
  
  printf("Terminating DkCst\n");
  DkCst_terminate();
  printf("Terminated DkCst\n");
  
  return 0; 
  
}
