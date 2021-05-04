#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
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
    IdeState(int h_cell_field, int w_cell, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Input *inpIo, Fl_Scroll *scrollTape,Fl_Pack *packTape);
//  private:
    void highlight_cell(unsigned cell);
    void unhighlight_cell(unsigned cell);
    void edit_program();

    unsigned char input();
    bool input_ready();
    void output(unsigned char out);
    void err_output(std::string message, bool is_warning);

    signed char d_handle();
    bool d_backhandle();
    void d_add_cell();
    void d_write_cell(unsigned char val);
    void d_write_tape_pos(unsigned oldPos);
    void d_write_prog_pos(unsigned oldPos);
    void d_clear_tape();

    const struct CellConfig config;
    Fl_Text_Editor *editor;
    Fl_Text_Display *dispIo;
    Fl_Input *inpIo;
    Fl_Scroll *scrollTape;
    Fl_Pack *packTape;
    bool dirty;
    bool isInput;
    bool wasRun;
    bool blocking;
    bool prompt;
    signed char lastStep;
};
void run_fwd_cb(Fl_Widget *w, void *p);
void run_back_cb(Fl_Widget *w, void *p);
void step_fwd_cb(Fl_Widget *w, void *p);
void step_back_cb(Fl_Widget *w, void *p);
void edited_cb(int pos, int nInserted, int nDeleted, int nRestyled,
               const char* deletedText,
               void* p);
void inp_edited_cb(Fl_Widget *w, void *p);
void prompt_cb(Fl_Widget *w, void *p);
void null_cb(Fl_Widget *w, void *p);