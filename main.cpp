#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_File_Chooser.H>
#include "bfide.h"


/* config macros */
#define W_WINDOW 800
#define H_WINDOW 600
#define H_MENUBAR 30
#define W_BUTTON 150
#define H_BUTTON 30
#define W_DISP 200
#define H_CELL_FIELD 20
#define W_CELL 40
#define MIN_W_EDITOR 200

/* convenience macros */
#define H_CELL 3*H_CELL_FIELD
#define H_TAPE (H_CELL + Fl::scrollbar_size())
#define W_EDITOR W_WINDOW - W_DISP
#define H_EDITOR H_WINDOW - H_TAPE - H_MENUBAR
#define H_DISP H_EDITOR - H_BUTTON
#define MIN_H_EDITOR MIN_W_EDITOR



void run_fwd_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->run_fwd();
}

void run_back_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->run_back();
}

void step_fwd_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->step_fwd();
}

void step_back_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->step_back();
}

void prompt_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->prompt(true);
}

void null_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->prompt(false);
}

void edited_cb(int pos, int nInserted, int nDeleted, int nRestyled,
               const char* deletedText,
               void* p)
{
  IdeState *state = *((IdeState**) p);
  if(nInserted || nDeleted)
    state->mark_dirty();
}

void inp_edited_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->unblock();
}

void save_as_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->export_file();
}

void open_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->import_file();
}

void save_cb(Fl_Widget *w, void *p)
{
  IdeState *state = *((IdeState**) p);
  state->export_curr();
}

int main(int argc, char **argv)
{
  /* widget hierarchy */
  Fl_Double_Window *window = new Fl_Double_Window(W_WINDOW,H_WINDOW,"bfide");
    Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,W_WINDOW,H_MENUBAR);
    Fl_Text_Editor *editor = new Fl_Text_Editor(0,H_MENUBAR,W_EDITOR,H_EDITOR);
    Fl_Group *groupIo = new Fl_Group(W_EDITOR,H_MENUBAR,W_DISP,H_EDITOR);
      Fl_Text_Display *dispIo = new Fl_Text_Display(W_EDITOR,H_MENUBAR,W_DISP,H_DISP);
      Fl_Input *inpIo = new Fl_Input(W_EDITOR,H_MENUBAR+H_DISP,W_DISP,H_BUTTON);
    groupIo->end();
    Fl_Scroll *scrollTape = new Fl_Scroll(0,H_MENUBAR+H_EDITOR,W_WINDOW,H_TAPE);
      Fl_Pack *packTape = new Fl_Pack(scrollTape->x(),scrollTape->y(),1,H_CELL);
      packTape->end();
    scrollTape->end();
  window->end();

  /* configure widgets and resize behaviour */
  packTape->type(Fl_Pack::HORIZONTAL);
  scrollTape->resizable(0);
  groupIo->resizable(dispIo);
  window->resizable(editor);
  window->size_range(MIN_W_EDITOR+W_DISP,MIN_H_EDITOR + H_TAPE);

  /* setup data structures */
  Fl_Text_Buffer *buffProg = new Fl_Text_Buffer();
  editor->buffer(buffProg);
  editor->textfont(FL_COURIER);
  Fl_Text_Buffer *buffIo = new Fl_Text_Buffer();
  dispIo->buffer(buffIo);
  dispIo->textfont(FL_COURIER);

  /* load commandline scripts or stdin, if any */
  char *openfile = 0;
  int i;
  int arg = Fl::args(argc,argv,i);
  if(arg && arg < argc)
  {
    if(argv[i][0]=='-')
    {
      char buff[1024];
      while(fgets(buff,1024,stdin))
        buffProg->append(buff);
    }
    else
      openfile = argv[i];
  }
  window->show(argc, argv);

  /* set up state object, menu, and callbacks */
  IdeState *statep = 0;
  Fl_Menu_Item menuItems[] =
    {
      {"&File",            0, 0,            0, FL_SUBMENU},
        {"&Open",          (FL_CTRL+'o'), open_cb,      &statep},
        {"&Save",          (FL_CTRL+'s'), save_cb,      &statep},
        {"Save &as",       (FL_CTRL+'S'), save_as_cb,   &statep},
      {0},
      {"&Run",             0, 0,            0, FL_SUBMENU},
        {"&Run",           (FL_F+4), run_fwd_cb,   &statep},
        {"Run back",       (FL_F+1), run_back_cb,  &statep, FL_MENU_DIVIDER},
        {"&Step",          (FL_F+3), step_fwd_cb,  &statep},
        {"Step &back",     (FL_F+2), step_back_cb, &statep},
      {0},
      {"&Options",         0, 0,            0, FL_SUBMENU},
        {"Get more input", 0, 0,            0, FL_SUBMENU},
          {"&Prompt user", (FL_CTRL+'p'), prompt_cb,    &statep, FL_MENU_RADIO|FL_MENU_VALUE},
          {"&Always null", (FL_CTRL+'n'), null_cb,      &statep, FL_MENU_RADIO},
        {0},
      {0},
    {0}};
  menu->menu(menuItems);
  IdeState state(H_CELL_FIELD,W_CELL,window,editor,dispIo,inpIo,scrollTape,packTape,menuItems+2,openfile);
  statep = &state;
  buffProg->add_modify_callback(&edited_cb, &statep);
  inpIo->callback(&inp_edited_cb, &statep);
  inpIo->when(FL_WHEN_CHANGED);

  return Fl::run();
}
