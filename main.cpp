#include <cstdlib>
#include "Scene.h"

using namespace std;

int main(int argc, char *args[]) {
  if(argc<2) {
    printf("\nNot enough arguments\n");
    return 1;
  }
  char *filename = args[1];
  Scene scene = Scene(string(filename)); 
  scene.Render();
  return 0;
}
