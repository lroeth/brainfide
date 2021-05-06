#include <string>
#include <vector>
#include <map>

class BFInt
{
  public:
    /* main interface */
    BFInt();
    bool update_program(std::string program);
    void reset_exec();
    int step(); /* returns -1 for error stop, 1 for eof, 2 for input not ready, positive for d_handle status, 0 for continue */

    static std::string export_c(std::string program);

  protected:
    /* mostly internal functions
    * protected for use by d_handle and d_backhandle in custom commands
    */
    unsigned get_prog_pos();
    unsigned get_tape_pos();
    int get_cmd(unsigned pos);
    unsigned char get_cell(unsigned pos);
    void write_cell(unsigned char val);
    void write_tape_pos(unsigned newPos);
    void write_prog_pos(unsigned newPos);

    /* virtuals for implementations */
    virtual unsigned char input()=0;
    virtual bool input_ready() {return true;}
    virtual void output(unsigned char out)=0;
    virtual void err_output(std::string message, bool is_error)=0;

    virtual int d_handle() {return -1;} /* return negative to indicate unhandled character, 1 for eof, positive to stop, 0 to continue */
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
};
