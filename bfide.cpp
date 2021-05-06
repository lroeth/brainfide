#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "bfide.h"


/* Public *********************************************************************/


CellConfig::CellConfig(int h_cell_field, int w_cell) : h_cell_field(h_cell_field),w_cell(w_cell)
{}


IdeState::IdeState(int h_cell_field, int w_cell, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Input *inpIo, Fl_Scroll *scrollTape,Fl_Pack *packTape, const char *openfile) :
  config(h_cell_field,w_cell),
  editor(editor),
  dispIo(dispIo),
  inpIo(inpIo),
  scrollTape(scrollTape),
  packTape(packTape),
  chooser(0),
  isDirty(false),
  isInput(false),
  isPrompt(true),
  lastStep(1),
  openfile(openfile)
{
  if(openfile)
    import_file(openfile);
  reset_exec();
}


IdeState::~IdeState()
{
  delete chooser;
}


void IdeState::step_fwd()
{
  if(clean())
    return;
  isRun=false;
  lastStep = step();
  if(lastStep == 1 || lastStep < 0)
    reset_exec();
  else if(lastStep == 2)
    block();
}


void IdeState::step_back()
{
  isBlocking=false;
  if(!clean())
    backstep();
}


void IdeState::run_fwd()
{
  if(clean()<0)
    return;
  if(lastStep == 1 || lastStep<0)
    reset_exec();
  isRun=true;
  while(!(lastStep = step()));
  if(lastStep == 2)
    block();
}


void IdeState::run_back()
{
  isBlocking=false;
  if(!clean())
    while(backstep());
}


void IdeState::unblock()
{
  if(isBlocking==false)
    return;
  isBlocking=false;
  if(isRun)
    run_fwd();
  else
    step_fwd();
}


void IdeState::mark_dirty()
  {isDirty=true;}


void IdeState::prompt(bool isPrompt)
  {this->isPrompt=isPrompt;}


void IdeState::export_file(const char *filename)
{
  if(!filename)
  {
    if(!chooser)
    {
      chooser = new Fl_File_Chooser(0,"BF source (*.bf)\tC source (*.c)\tAll Files (*)",Fl_File_Chooser::CREATE,"Save as");
      chooser->callback(&chooser_cb, this);
    }
    else
    {
      chooser->type(Fl_File_Chooser::CREATE);
      chooser->label("Save as");
      chooser->value(openfile);
      chooser->filter("BF source (*.bf)\tC source (*.c)\tAll Files (*)");
    }
    chooser->show();
  }
  else
  {
    if(chooser->filter_value() == 1)
    {
      //cprog
    }
    else
      editor->buffer()->savefile(filename);
    openfile = filename;
  }
}


void IdeState::import_file(const char *filename)
{
  if(!filename)
  {
    if(!chooser)
    {
      chooser = new Fl_File_Chooser(openfile,"BF Source (*.bf)\tAll Files (*)",Fl_File_Chooser::SINGLE,"Open");
      chooser->callback(&chooser_cb, this);
    }
    else
    {
      chooser->type(Fl_File_Chooser::SINGLE);
      chooser->label("Open");
      chooser->value(openfile);
      chooser->filter("BF Source (*.bf)\tAll Files (*)");
    }
    chooser->show();
  }
  else
  {
    editor->buffer()->loadfile(filename);
    mark_dirty();
    openfile = filename;
  }
}


/* Private ********************************************************************/


void IdeState::block()
{
  isBlocking = true;
  inpIo->take_focus();
}


int IdeState::clean()
{
  if(!isDirty)
    return 0;
  char *buff = editor->buffer()->text();
  if(!update_program(buff))
    return -1;
  free(buff);
  reset_exec();
  isDirty = false;
  return 1;
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


unsigned char IdeState::input()
{
  if(!isInput)
  {
    /* make buffer ready to have input written to it */
    char buff[3] = "\n>";
    dispIo->buffer()->append(buff);
    isInput=true;
  }
  int size;
  if(!(size=inpIo->size()))
    /* input never gets called if set to prompt with no input ready.
    *  so if execution gets here, and none is ready, send null. */
    return 0;
  char buff[2];
  buff[0] = inpIo->value()[0];
  /* consume character */
  inpIo->replace(0,1,0);
  inpIo->position(size-1);
  buff[1]='\0';
  dispIo->buffer()->append(buff);
  return buff[0];  
}


bool IdeState::input_ready()
{
  if(!isPrompt)
    return true;
  return (inpIo->size() > 0);
}


void IdeState::output(unsigned char out)
{
  char buff[3];
  buff[2]='\0';
  buff[1]=out;
  buff[0]='\n';
  /* put output on separate line from input */
  dispIo->buffer()->append(isInput ? buff : buff+1);
  isInput=false;
}


void IdeState::err_output(std::string message, bool is_warning)
{
//make popup later?
  if(!is_warning)
    dispIo->buffer()->append(("\nERROR: "+message).c_str());
}




int IdeState::d_handle()
{
  /* handle breakpoints */
  return get_cmd(get_prog_pos()) == '$' ? 3 : 0;
}


bool IdeState::d_backhandle()
{
  /* handle reverse breakpoints */
  return get_cmd(get_prog_pos()) == '$' ? false : true;
}


void IdeState::d_add_cell()
{
  /* add widget */
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
  /* write 0 */
  d_write_cell(0);
}


void IdeState::d_write_cell(unsigned char val)
{
  Fl_Group *cell=packTape->child(get_tape_pos())->as_group();
  Fl_Output *cellMemb = (Fl_Output*) cell->child(0);
  char buff[3] = "";
  /* ascii */
  snprintf(buff,3,"%c",val);
  cellMemb->value(buff);
  cellMemb = (Fl_Output*) cell->child(1);
  /* hex */
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
  /* clears dispIo: for history, change to insert some newlines */
  dispIo->buffer()->text(0);
  isInput=false;
  packTape->clear();
  d_add_cell();
  highlight_cell(0);
  lastStep=0;
}


void IdeState::chooser_cb(Fl_File_Chooser *w, void *p)
{
  IdeState *state = (IdeState*) p;
  if(!w->shown())
  {
    switch(w->type())
    {
      case Fl_File_Chooser::SINGLE : state->import_file(w->value());break;
      case Fl_File_Chooser::CREATE : state->export_file(w->value());break;
    }
  }
}
