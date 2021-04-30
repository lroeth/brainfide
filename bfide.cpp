#include <stdio.h>
#include <FL/Fl_Output.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "bfide.h"

CellConfig::CellConfig(int h_cell_field, int w_cell) : h_cell_field(h_cell_field),w_cell(w_cell)
{}

IdeState::IdeState(int h_cell_field, int w_cell, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Scroll *scrollTape,Fl_Pack *packTape) :
  config(h_cell_field,w_cell),
  editor(editor),
  dispIo(dispIo),
  scrollTape(scrollTape),
  packTape(packTape)
{}




unsigned char IdeState::input()
{
  static int charnum=0;
  static const std::string myStr = "He!llo";
  return myStr[((charnum++)%6)];
}

void IdeState::output(unsigned char out)
{
  char buff[2];
  buff[1]='\0';
  buff[0]=out;
  dispIo->buffer()->append(buff);
  dispIo->redraw();
}

void IdeState::err_output(std::string message, bool is_warning)
{
}

void IdeState::d_add_cell()
{
  const int h_cell_field = config.h_cell_field;
  const int h_cell = 3 * h_cell_field;
  const int w_cell = config.w_cell;
  Fl_Group *newCell = new Fl_Group(0,0,w_cell,h_cell);
    Fl_Output *newCellChar = new Fl_Output(0,0,w_cell,h_cell_field);
    Fl_Output *newCellVal = new Fl_Output(0,h_cell_field,w_cell,h_cell_field);
    Fl_Box *newCellIndex = new Fl_Box(0,2*h_cell_field,w_cell,h_cell_field);
  newCell->end();
  packTape->add(newCell);
  char buff[1024] = "";
  snprintf(buff,1024,"%i",get_tape_pos());
  newCellIndex->copy_label(buff);
  scrollTape->redraw();
}

void run_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  char *buff = state->editor->buffer()->text();
  state->update_program(buff);
  char *curr = buff;
  for(;*curr!='\0';curr++)
    state->output(*curr);
  free(buff);
}





//  snprintf(buff,1024,"%c",);
//  newCellChar->value(buff);
//  snprintf(buff,1024,"%2X",);
//  newCellVal->value(buff);
//  scroll->redraw();
