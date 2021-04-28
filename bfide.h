#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>

struct CellConfig
{
  int h_cell_field;
  int w_cell;
  CellConfig(int h_cell_field, int w_cell);
};

struct State
{
  unsigned char nextval;
  int numCells;
  const struct CellConfig config;
  Fl_Pack *tape;
  Fl_Scroll *scroll;

  State(int h_cell_field, int w_cell,Fl_Pack *tape, Fl_Scroll *scroll);
  void addCell();
};

void addCellCb(Fl_Widget *w, void *p);
