#include <iostream>
#include <fstream>
#include "bfint.h"

int main(int argc, char **argv)
{
  if(argc<2)
    return 1;
  std::ifstream prog;
  prog.open(argv[1]);
  std::string curr;
  std::string program;
  while(getline(prog,curr))
  {
    program+=curr;
    program+='\n';
  }
  State state;
  if(!state.update_program(program))
    return 2;
  int status;
  while((status = state.step())==0);
  if(status < 0)
    return 2;
  else
    return 0;
}
