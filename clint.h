#include "bfint.h"

class CLInt : public BFInt
{
  private:
    unsigned char input();
    void output(unsigned char out);
    void err_output(std::string message, bool is_warning);
    int d_handle();
};
