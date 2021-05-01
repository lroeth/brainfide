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
  packTape(packTape),
  dirty(false),
  lastStep(1)
{
  reset_exec();
}




void IdeState::highlight_cell(unsigned cell)
{
  if(cell >= packTape->children())
    return;
  Fl_Group *cellGroup = packTape->child(cell)->as_group();
  Fl_Widget *cellMemb = cellGroup->child(2);
  cellMemb->box(FL_UP_BOX);
}

void IdeState::unhighlight_cell(unsigned cell)
{
  if(cell >= packTape->children())
    return;
  Fl_Group *cellGroup = packTape->child(cell)->as_group();
  Fl_Widget *cellMemb = cellGroup->child(2);
  cellMemb->box(FL_NO_BOX);
}

void IdeState::edit_program()
{
  char *buff = editor->buffer()->text();
  update_program(buff);
  free(buff);
  reset_exec();
  dirty = false;
}

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
  if(!is_warning)
    dispIo->buffer()->append(("\nERROR: "+message).c_str());
}

signed char IdeState::d_handle()
{
  return get_cmd(get_prog_pos()) == '$' ? 2 : 0;
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
  char buff[20] = "";
  snprintf(buff,20,"%i",get_tape_pos());
  newCellIndex->copy_label(buff);
  d_write_cell(0);
}

void IdeState::d_write_cell(unsigned char val)
{
  Fl_Group *cell=packTape->child(get_tape_pos())->as_group();
  Fl_Output *cellMemb = (Fl_Output*) cell->child(0);
  char buff[3] = "";
  snprintf(buff,3,"%c",val);
  cellMemb->value(buff);
  cellMemb = (Fl_Output*) cell->child(1);
  snprintf(buff,3,"%2X",val);
  cellMemb->value(buff);
  scrollTape->redraw();
}

void IdeState::d_write_tape_pos(unsigned oldPos)
{
  unhighlight_cell(oldPos);
  highlight_cell(get_tape_pos());
  scrollTape->redraw();
}

void IdeState::d_write_prog_pos(unsigned oldPos)
  {editor->buffer()->highlight(get_prog_pos(),get_prog_pos()+1);}

void IdeState::d_clear_tape()
{
  dispIo->buffer()->text(0);
  packTape->clear();
  d_add_cell();
  highlight_cell(0);
}

void run_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  if(state->dirty || state->lastStep<0)
    state->edit_program();
  else if(state->lastStep == 1)
    state->reset_exec();
  while(!(state->lastStep = state->step()));
}

void step_fwd_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  if(state->dirty)
    state->edit_program();
  else
  {
    state->lastStep = state->step();
    if(state->lastStep == 1 || state->lastStep < 0)
    {
      state->reset_exec();
      state->lastStep = 0;
    }
  }
}

void edited_cb(int pos, int nInserted, int nDeleted, int nRestyled,
               const char* deletedText,
               void* p)
{
  IdeState *state = (IdeState*) p;
  state->dirty |= (nInserted || nDeleted);
}
