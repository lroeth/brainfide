#include <string>
#include <vector>
#include <map>
#include <stack>
#include <utility>

class BFInt
{
  public:
    BFInt();
    bool update_program(std::string program);
    void reset_exec();
    signed char step(); /* returns -1 for error stop, 1 for eof, 2 for input not ready, positive for d_handle status, 0 for continue */
    bool backstep(); /* returns true if not at beginning of file */
  protected:
    unsigned get_prog_pos();
    unsigned get_tape_pos();
    int get_cmd(unsigned pos);
    unsigned char get_cell(unsigned pos);
    void write_cell_inp(unsigned char val);
    void write_cell(unsigned char val);
    void write_tape_pos(unsigned newPos);
    void write_prog_pos(unsigned newPos);

    virtual unsigned char input()=0;
    virtual bool input_ready() {return true;}
    virtual void output(unsigned char out)=0;
    virtual void err_output(std::string message, bool is_error)=0;

  private:

    virtual signed char d_handle() {return -1;} /* return negative to indicate unhandled character, 1 for eof, positive to stop, 0 to continue */
    virtual void d_backhandle() {}
    virtual void d_add_cell() {}
    virtual void d_write_cell(unsigned char val) {}
    virtual void d_write_tape_pos(unsigned oldPos) {}
    virtual void d_write_prog_pos(unsigned oldPos) {}
    virtual void d_clear_tape() {}

    unsigned progPos;
    unsigned tapePos;
    std::string program;
    std::vector<unsigned char> tape;
    std::map<unsigned,unsigned> loops;
    std::stack<unsigned char> overwrite;
    std::map<unsigned,std::pair<std::stack<unsigned>,bool> > loopReps;
};
