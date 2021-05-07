#ifndef CLINT_H
#define CLINT_H

#include "bfint.h"

class CLInt : public BFInt
{
  public:
    CLInt(std::string *program);
  private:
    unsigned char input();
    void output(unsigned char out);
    void err_output(std::string message, bool is_warning);
    std::string new_program();
    int d_handle();
    std::string *program;
};

#endif //CLINT_H
