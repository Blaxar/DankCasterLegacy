#include <stdlib.h>
#include <stdio.h>
#include <libdkc/dkc_app.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
  
  printf("Initializing Dkc\n");
  dkc_init();
  printf("Initialized Dkc\n");

  DkcApp* app;
  
  if(!dkc_rc_ok(dkc_create_app(&app))) printf("Failed to create app.\n");
  
  printf("Terminating Dkc\n");
  dkc_terminate();
  printf("Terminated Dkc\n");
  
  return 0; 
  
}
