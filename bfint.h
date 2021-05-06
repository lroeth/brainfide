#include <string>
#include <vector>
#include <map>
#include <stack>

class BFInt
{
  public:
    /* main interface */
    BFInt();
    bool update_program(std::string program);
    void reset_exec();
    int step(); /* returns -1 for error stop, 1 for eof, 2 for input not ready, positive for d_handle status, 0 for continue */
    bool backstep(); /* returns false if not at beginning of file or d_backhandle returned false */

    static std::string export_c(std::string program);

  protected:
    /* mostly internal functions
    * protected for use by d_handle and d_backhandle in custom commands
    * When changing tape cell via function f(x):
    *   if f is invertible, write with write_cell(f(x)), and undo with write_cell(f^-1(get_cell(get_tape_pos())))
    *   otherwise, write with write_cell_inp(f(x)), and undo with unwrite_cell_inp().
    */
    unsigned get_prog_pos();
    unsigned get_tape_pos();
    int get_cmd(unsigned pos);
    unsigned char get_cell(unsigned pos);
    void write_cell(unsigned char val);
    void write_cell_inp(unsigned char val);
    void unwrite_cell_inp();
    void write_tape_pos(unsigned newPos);
    void write_prog_pos(unsigned newPos);

    virtual unsigned char input()=0;
    virtual bool input_ready() {return true;}
    virtual void output(unsigned char out)=0;
    virtual void err_output(std::string message, bool is_error)=0;

  private:
    virtual int d_handle() {return -1;} /* return negative to indicate unhandled character, 1 for eof, positive to stop, 0 to continue */
    virtual bool d_backhandle() {return true;} /* return true to not stop */
    virtual void d_add_cell() {}
    virtual void d_write_cell(unsigned char val) {}
    virtual void d_write_tape_pos(unsigned oldPos) {}
    virtual void d_write_prog_pos(unsigned oldPos) {}
    virtual void d_clear_tape() {}

    void backstep_loop(unsigned left, unsigned right);
    bool step_loop(unsigned left, unsigned right, bool isLeft);

    unsigned progPos;
    unsigned tapePos;
    std::string program;
    std::vector<unsigned char> tape;
    std::map<unsigned,unsigned> loops;
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
