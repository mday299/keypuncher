#include <hellomake.h>
#include <math.h>

int main() {
  double cosRes = -99.9;

  // call a function in another file
  myPrintHelloMake();

  // exercise math.h
  cosRes = cos(1);

  return(0);
}
