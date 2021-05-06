#include "bfint.h"
#include <stack>

class BFIntBidir : public BFInt
{
  public:
    /* overrides to record needed information */
    void reset_exec();
    int step();
    /* inverse of step */
    bool backstep(); /* returns false if not at beginning of file or d_backhandle returned false */

  protected:
    /* invertible version of write_cell(), and corresponding inverse */
    void write_cell_inp(unsigned char val);
    void unwrite_cell_inp();

    /* inverse of input() and output() */
    virtual void backinput()=0;
    virtual void backoutput()=0;

    /* inverse of d_handle() */
    virtual bool d_backhandle() {return true;} /* return true to not stop */

    /* convenience for making step() and backstep() invertible */
    void backstep_loop(unsigned left, unsigned right);
    bool step_loop(unsigned left, unsigned right, bool isLeft);

    std::stack<unsigned char> overwrite;
    /* loopReps invariants
    *  One map entry per pair of '[]'.
    *  Keys are progPos of the '['.
    *  Each stack has one element for each separate entry of the loop.
    *  This element is pushed/popped on the boundary when progPos goes from on
    *    the '[' to after it.
    *  The element is the number of times the loop has been executed.
    *  It is incremented on the boundary when progPos goes from on the ']' to
    *    after it, or the boundary when progPos goes from on the ']' to after
    *    the '['.
    */
    std::map<unsigned,std::stack<unsigned> > loopReps;
};
