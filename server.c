#include "server.h"

int main() {
  DIR* dir = opendir("projects/demo_proj");
  if (dir) {
    printf("dir exists\n");
  }
  else if (ENOENT == errno) {
    printf("dir does not exist\n");
  }
  else {
    perror("something wrong\n");
  }

  return 0;
}
