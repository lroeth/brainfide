#include <stdio.h>
#include <FL/Fl_Output.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "bfide.h"

CellConfig::CellConfig(int h_cell_field, int w_cell) : h_cell_field(h_cell_field),w_cell(w_cell)
{}

State::State(int h_cell_field, int w_cell,Fl_Pack *tape,Fl_Scroll *scroll) :
  config(h_cell_field,w_cell),
  nextval('z'),
  numCells(0),
  tape(tape),
  scroll(scroll)
{}

void State::addCell()
{
  const int h_cell_field = config.h_cell_field;
  const int h_cell = 3 * h_cell_field;
  const int w_cell = config.w_cell;
  Fl_Group *newCell = new Fl_Group(0,0,w_cell,h_cell);
    Fl_Output *newCellChar = new Fl_Output(0,0,w_cell,h_cell_field);
    Fl_Output *newCellVal = new Fl_Output(0,h_cell_field,w_cell,h_cell_field);
    Fl_Box *newCellIndex = new Fl_Box(0,2*h_cell_field,w_cell,h_cell_field);
  newCell->end();
  tape->add(newCell);
  char buff[1024] = "";
  snprintf(buff,1024,"%i",numCells++);
  newCellIndex->copy_label(buff);
  snprintf(buff,1024,"%c",nextval);
  newCellChar->value(buff);
  snprintf(buff,1024,"%2X",nextval++);
  newCellVal->value(buff);
  scroll->redraw();
}

void addCellCb(Fl_Widget *w, void *p)
{
  State *state = (State*) p;
  state->addCell();
}
