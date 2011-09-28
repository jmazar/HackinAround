#include <stdio.h>

class BaseClass
{
public:
  virtual void VA(){
    printf("VA\n");
  }
  virtual void VB(){
    printf("VB\n");
  };
};


class SubClassA : public BaseClass
{
public:
  virtual void VC(){
    printf("VC\n");
  };
  virtual void VD(){
    printf("VD\n");
  };
};

class SubClassB : public SubClassA
{
public:
  virtual void VE(){
    //This crashes at runtime
    //VA();
    printf("VE\n");
  }
};