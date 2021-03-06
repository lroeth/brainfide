#include <stdio.h>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/filename.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Item.H>
#include "idestate.h"


/* Public *********************************************************************/


CellConfig::CellConfig(int h_cell_field, int w_cell) : h_cell_field(h_cell_field),w_cell(w_cell)
{}

IdeState::IdeState(int h_cell_field, int w_cell, Fl_Window *window, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Input *inpIo, Fl_Scroll *scrollTape,Fl_Pack *packTape,Fl_Menu_Item *menuSave,const char *openfile) :
  window(window),
  editor(editor),
  dispIo(dispIo),
  inpIo(inpIo),
  scrollTape(scrollTape),
  packTape(packTape),
  chooser(0),
  menuSave(menuSave),
  config(h_cell_field,w_cell),
  isDirtyFile(false),
  isInput(false),
  isPrompt(true),
  openfile(openfile)
{
  if(openfile)
    open(openfile);
  else
    mark_dirty_file(false);
  reset_exec();
}


IdeState::~IdeState()
{
  delete chooser;
}


void IdeState::mark_dirty()
{
  BFInt::mark_dirty();
  mark_dirty_file(true);
}


void IdeState::prompt()
  {isPrompt = true;}


void IdeState::null()
{
  isPrompt = false;
  unblock();
}


void IdeState::new_file()
{
  if(overwrite())
    return;
  reset_exec();
  editor->buffer()->text(0);
  BFInt::mark_dirty();
  openfile = 0;
  mark_dirty_file(false);
}


void IdeState::save_as(const char *filename)
{
  if(!filename)
  {
    create_chooser("BF source (*.bf)\tC source (*.c)\tAll Files (*)",
                   Fl_File_Chooser::CREATE,"Save as");
  }
  else
  {
    if((!openfile || strcmp(openfile,filename)) &&
        chooser && chooser->filter_value() == 1)
    {
      FILE *cFile = fl_fopen(filename,"w");
      char *buff = editor->buffer()->text();
      fputs(BFInt::export_c(buff).c_str(),cFile);
      free(buff);
      fclose(cFile);
    }
    else
    {
      editor->buffer()->savefile(filename);
      openfile = filename;
      mark_dirty_file(false);
    }
  }
}


void IdeState::save()
  {save_as(openfile);}


void IdeState::open(const char *filename)
{
  if(!filename)
  {
    if(overwrite())
      return;
    create_chooser("BF Source (*.bf)\tAll Files (*)",
                   Fl_File_Chooser::SINGLE,"Open");
  }
  else
  {
    editor->buffer()->loadfile(filename);
    BFInt::mark_dirty();
    openfile = filename;
    mark_dirty_file(false);
  }
}

void IdeState::close()
{
  if(overwrite())
    return;
  window->hide();
}


/* Private ********************************************************************/


bool IdeState::overwrite()
{
  if(!isDirtyFile)
    return false;
  switch(fl_choice("You have unsaved changes to %s. Are you sure?",
                   "Cancel","Continue","Save",openfile ? openfile : "untitled"))
    {
      case 0: return true;
      case 2: save();
      case 1:
      default: return false;
    }
}


void IdeState::update_title()
{
  char buff[FL_PATH_MAX + 20];
  strcpy(buff,"bfide - ");
  if(isDirtyFile)
    strcat(buff,"*");
  if(openfile)
    strncat(buff,fl_filename_name(openfile),FL_PATH_MAX);
  else
    strcat(buff,"untitled");
  window->copy_label(buff);
}


void IdeState::mark_dirty_file(bool isDirty)
{
  isDirtyFile = isDirty;
  if(!openfile || isDirtyFile)
    menuSave->activate();
  else
    menuSave->deactivate();
  update_title();
}


void IdeState::create_chooser(const char *filter, int type, const char *label)
{
  if(!chooser)
  {
    chooser = new Fl_File_Chooser(openfile,filter,type,label);
    chooser->callback(&chooser_cb, this);
  }
  else
  {
    chooser->type(type);
    chooser->label(label);
    chooser->filter(filter);
    chooser->value(openfile);
  }
  chooser->show();
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

void IdeState::backinput()
{
  bool isInputBegin = get_prog_pos() == 0;
  int toDelete = !isInputBegin ? isInput ? 1 : 2 : 4;
  isInput = !isInputBegin;
  int end = dispIo->buffer()->length();
  char cell = get_cell(get_tape_pos());
  if(cell)
  {
    inpIo->position(0);
    inpIo->insert(&cell, 1);
    inpIo->position(inpIo->size());
    dispIo->buffer()->remove(end - toDelete, end);
  }
  else
    dispIo->buffer()->remove(end - toDelete - 1, end);

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

void IdeState::backoutput()
{
  int toDelete = isInput ? 3 : 1;
  isInput=false;
  int end = dispIo->buffer()->length();
  dispIo->buffer()->remove(end - toDelete, end);
}

void IdeState::err_output(std::string message, bool is_warning)
{
//make popup later?
  if(!is_warning)
    fl_message(("\nERROR: "+message).c_str());
}

std::string IdeState::new_program()
  {return std::string(editor->buffer()->text());}


void IdeState::d_block()
  {inpIo->take_focus();}


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


void IdeState::d_reset_exec()
{
  /* clears dispIo: for history, change to insert some newlines */
  dispIo->buffer()->text(0);
  isInput=false;
  packTape->clear();
  d_add_cell();
  highlight_cell(0);
}


void IdeState::chooser_cb(Fl_File_Chooser *w, void *p)
{
  IdeState *state = (IdeState*) p;
  if(!w->shown())
  {
    switch(w->type())
    {
      case Fl_File_Chooser::SINGLE : state->open(w->value());break;
      case Fl_File_Chooser::CREATE : state->save_as(w->value());break;
    }
  }
}
