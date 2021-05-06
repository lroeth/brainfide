#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Input.H>
#include "bfint.h"

struct CellConfig
{
  int h_cell_field;
  int w_cell;
  CellConfig(int h_cell_field, int w_cell);
};

class IdeState : public BFInt
{
  public:
    IdeState(int h_cell_field, int w_cell, Fl_Window *window, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Input *inpIo, Fl_Scroll *scrollTape,Fl_Pack *packTape,const char *openfile);
    ~IdeState();
    /* Execute/rewind one instruction */
    void step_fwd();
    void step_back();
    /* Execute/rewind until end/beginning or breakpoint */
    void run_fwd();
    void run_back();
    /* Stop blocking for input, because it is available */
    void unblock();
    /* Reload program */
    void mark_dirty();
    /* Set behavior on running out of input:
    *    true  | prompt user for more input
    *    false | send null character ('\0')
    */
    void prompt(bool isPrompt);
    /* Export / import script to/from file
    *  If filename is null, run file picker to get file
    *  Export_curr overwrites currently open filename automatically,
    *    or calls file picker if no currently open file.
    */
    void export_file(const char *filename=0);
    void export_curr();
    void import_file(const char *filename=0);

    void update_title();
  private:
    /* block for user input */
    void block();

    /* check for new program, parse it and reset_exec if present
    *  1 was dirty, clean now
    *  0 was clean
    *  -1 was dirty, still dirty (update_program failed)
    */
    int clean();

    /* handle tape pointer, for d_write_tape_pos */
    void highlight_cell(unsigned cell);
    void unhighlight_cell(unsigned cell);


    /* I/O virtual functions from bfint */
    unsigned char input();
    bool input_ready();
    void output(unsigned char out);
    void err_output(std::string message, bool is_warning);

    /* UI update virtual functions from bfint*/
    int d_handle();
    bool d_backhandle();
    void d_add_cell();
    void d_write_cell(unsigned char val);
    void d_write_tape_pos(unsigned oldPos);
    void d_write_prog_pos(unsigned oldPos);
    void d_clear_tape();

    /* callback for File_Chooser */
    static void chooser_cb(Fl_File_Chooser *w, void *p);

    /* UI components needed by member functions */
    Fl_Window *window;
    Fl_Text_Editor *editor;
    Fl_Text_Display *dispIo;
    Fl_Input *inpIo;
    Fl_Scroll *scrollTape;
    Fl_Pack *packTape;
    Fl_File_Chooser *chooser;

    /* State data */
    const struct CellConfig config; /* information needed to construct cells */
    bool isDirty; /* program has been edited since last parse */
    bool isInput; /* output is ready for input */
    bool isPrompt; /* if input runs out, prompt user. if false, just feed nulls */
    bool isRun; /* before blocking for input, program was run, not stepped */
    bool isBlocking; /* currently blocking for input; ignore most callbacks */
    int lastStep; /* return status of last call to step() */
    const char *openfile; /* current file, or null for untitled */
};
