#include <FL/Fl_Text_Editor.H>
#include <Fl/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
#include <queue>
#include "bfint.h"

struct RO_Editor : public Fl_Text_Display
{
  RO_Editor(int X, int Y, int W, int H, const char* label=0) :
    Fl_Text_Display(X,Y,W,H,label),
    inpQ()
    {}
  int handle(int e);
  char getchar();
  std::queue<char> inpQ;
};

struct CellConfig
{
  int h_cell_field;
  int w_cell;
  CellConfig(int h_cell_field, int w_cell);
};

class IdeState : public BFInt
{
  public:
    IdeState(int h_cell_field, int w_cell, Fl_Text_Editor *editor, RO_Editor *dispIo, Fl_Scroll *scrollTape,Fl_Pack *packTape);
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

    const struct CellConfig config;
    Fl_Text_Editor *editor;
    RO_Editor *dispIo;
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
