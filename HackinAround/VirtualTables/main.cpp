#include "Inheritance.h"
#include <string.h>

typedef void (*MyFunc)();
MyFunc funcptr;

void callback()
{
  printf("I'll just slip this right here...\n");
  (*funcptr)();
}
int main()
{
  SubClassB* b = new SubClassB();

  b->VE();
  void** v_table = *(void***) b;

  int i = 0x10 / sizeof(void*);

  printf("The virtual table is %d bytes!\n",i + 1);

  funcptr = (MyFunc)v_table[4];

  b->VE();
  (*funcptr)();

  void* new_vtable[5];

  memcpy(new_vtable, v_table, sizeof(new_vtable));

  *(void**) b = new_vtable;

  new_vtable[4] = *callback;

  b->VE();

  return 0;
}