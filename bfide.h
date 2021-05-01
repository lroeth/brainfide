#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
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
    IdeState(int h_cell_field, int w_cell, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Scroll *scrollTape,Fl_Pack *packTape);
//  private:
    void highlight_cell(unsigned cell);
    void unhighlight_cell(unsigned cell);
    unsigned char input();
    void output(unsigned char out);
    void err_output(std::string message, bool is_warning);

    signed char d_handle();
    void d_add_cell();
    void d_write_cell(unsigned char val);
    void d_write_tape_pos(unsigned oldPos);
    void d_clear_tape();

    const struct CellConfig config;
    Fl_Text_Editor *editor;
    Fl_Text_Display *dispIo;
    Fl_Scroll *scrollTape;
    Fl_Pack *packTape;
};
void run_cb(Fl_Widget *w, void *p);
