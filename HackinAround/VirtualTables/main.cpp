#include "Inheritance.h"
#include <string.h>
#include <intrin.h>
typedef void (*DoSomethingFunc)();



void MyFunction()
{
	printf("Inside MyFunction!\n");
}

void __cdecl printme(int a)
{
	printf("%d\n", a);
}

int main()
{
	//Calling from the virtual table. Gets rid of indirection
	SubClassB* a = new SubClassB();

	void** v_table = *(void***) a;
	DoSomethingFunc myFunc = (DoSomethingFunc)(v_table[4]);

	a->VE();
	(*myFunc)();

	int i = 0x10 / sizeof(void*);
	printf("Size of virtual table is %d bytes.\n", i+1);

	//Stealing the virtual table for our own means

	void* new_vtable[5];
	// Copy v-table 
	memcpy(new_vtable, v_table, sizeof(new_vtable)); 
	// Replace v-table pointer 
	*(void **) a = new_vtable; 


	new_vtable[4] = &MyFunction;
	a->VE();
	
	SubClassB b;
	b.VE();
	__asm
	{
		lea ebx, a
		mov ecx, [ebx]

		mov eax, dword ptr [a]	   //Dereferences pointer a, now pointing at the top of the object in memory
		mov edx, dword ptr [eax]   //First four bytes a pointer to the virtual table, so dereferencing to the virtual table
		mov eax, dword ptr [edx]   //Now at the top of the virtual table, first four bytes a pointer to the first function of the virtual table
		call eax				   //Points to first function in the virtual table
		mov ebx, dword ptr [b]     //Same thing as above, just don't need the first level of indirection. Moving to virtual table
		mov eax, dword ptr [ebx]   //Going from virtual table to function
		call eax				   //Points to first function in the virtual table
		mov eax, dword ptr [b+0Ch] //Offset straight to virutal table. Found this by looking at disassembly and seeing it was using an offset
		call eax				   //Points to first function in the virtual table
	};
	return 0;
}