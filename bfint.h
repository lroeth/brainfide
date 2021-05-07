#include <string>
#include <vector>
#include <map>

class BFInt
{
  public:
    /* main interface */
    BFInt();
    virtual void reset_exec(); /* reset progPos, tape, etc. */
    bool step_fwd(); /* execute 1 instruction, return true unless clean failed */
    bool run_fwd(); /* run until end, or d_handle stop, return true unless clean failed */
    void unblock(); /* call when input is available */
    void mark_dirty(); /* call when program has changed */

    static std::string export_c(std::string program); /* convert a BF program to C source */

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
    virtual std::string new_program()=0;

    /* virtual extensions: called after the instruction
    *  can be used to show execution state, etc.
    */
    virtual void d_block() {}
    virtual int d_handle() {return -1;} /* return negative to indicate unhandled character, 1 for eof, positive to stop, 0 to continue */
    virtual void d_add_cell() {}
    virtual void d_write_cell(unsigned char val) {}
    virtual void d_write_tape_pos(unsigned oldPos) {}
    virtual void d_write_prog_pos(unsigned oldPos) {}
    virtual void d_reset_exec() {}


    virtual int step(); /* returns -1 for error stop, 1 for eof, 2 for input not ready, positive for d_handle status, 0 for continue */
    void block(); /* block for user input */
    /* check for new program, parse it and reset_exec if needed
    *  return:
    *    1 was dirty, clean now
    *    0 was already clean
    *    -1 was dirty, still dirty (update_program failed)
    */
    int clean();
    bool update_program(std::string program);
    unsigned progPos;
    unsigned tapePos;
    std::string program;
    std::vector<unsigned char> tape;
    std::map<unsigned,unsigned> loops;
    bool isDirty;
    bool isRun;
    bool isBlocking;
};
