#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Input.H>
#include "bfint.h"
#include "roeditor.h"

struct CellConfig
{
  int h_cell_field;
  int w_cell;
  CellConfig(int h_cell_field, int w_cell);
};

class IdeState : public BFInt
{
  public:
    IdeState(int h_cell_field, int w_cell, Fl_Text_Editor *editor, RO_Editor *dispIo, Fl_Input *inpIo, Fl_Scroll *scrollTape,Fl_Pack *packTape);
//  private:
    void highlight_cell(unsigned cell);
    void unhighlight_cell(unsigned cell);
    void edit_program();

    unsigned char input();
    void output(unsigned char out);
    void err_output(std::string message, bool is_warning);

    signed char d_handle();
    void d_add_cell();
    void d_write_cell(unsigned char val);
    void d_write_tape_pos(unsigned oldPos);
    void d_write_prog_pos(unsigned oldPos);
    void d_clear_tape();

    static char getchar(void *p);

    const struct CellConfig config;
    Fl_Text_Editor *editor;
    RO_Editor *dispIo;
    Fl_Input *inpIo;
    Fl_Scroll *scrollTape;
    Fl_Pack *packTape;
    bool dirty;
    bool isInput;
    signed char lastStep;
};
void run_cb(Fl_Widget *w, void *p);
void step_fwd_cb(Fl_Widget *w, void *p);
void edited_cb(int pos, int nInserted, int nDeleted, int nRestyled,
               const char* deletedText,
               void* p);
