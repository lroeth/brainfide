#include "clint.h"
#include <string.h>
#include <fstream>

unsigned char CLInt::input()
{
  char in = std::cin.get();
  if(!std::cin.good())
  {
    err_output("EOF on input",true);
    in='\0';
  }
  return in;
}

void CLInt::output(unsigned char out)
  {std::cout<<out;}

void CLInt::err_output(std::string message, bool is_warning)
  {std::cerr<<(is_warning?"WARNING: ":"ERROR: ")<<message<<std::endl;}

int CLInt::d_handle()
  {return 0;/* ignore non command characters */}



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
  CLInt state;
  if(!state.update_program(program))
    return 2;
  int status;
  while((status = state.step())==0);
  if(status < 0)
    return 2;
  else
    return 0;
}
