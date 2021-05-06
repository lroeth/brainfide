#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_File_Chooser.H>
#include "bfide.h"


/* config macros */
#define W_WINDOW 800
#define H_WINDOW 600
#define W_BUTTON 150
#define H_BUTTON 30
#define H_DISP 100
#define H_CELL_FIELD 20
#define W_CELL 40
#define MIN_W_EDITOR 200

/* convenience macros */
#define H_CELL 3*H_CELL_FIELD
#define H_TAPE (H_CELL + Fl::scrollbar_size())
#define W_EDITOR W_WINDOW - W_BUTTON
#define H_EDITOR H_WINDOW - H_DISP - H_TAPE




void run_fwd_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->run_fwd();
}

void run_back_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->run_back();
}

void step_fwd_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->step_fwd();
}

void step_back_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->step_back();
}

void prompt_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->prompt(true);
}

void null_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->prompt(false);
}

void edited_cb(int pos, int nInserted, int nDeleted, int nRestyled,
               const char* deletedText,
               void* p)
{
  IdeState *state = (IdeState*) p;
  if(nInserted || nDeleted)
    state->mark_dirty();
}

void inp_edited_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->unblock();
}

void export_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->export_file();
}

void import_cb(Fl_Widget *w, void *p)
{
  IdeState *state = (IdeState*) p;
  state->import_file();
}



int main(int argc, char **argv)
{
  /* widget hierarchy */
  Fl_Double_Window *window = new Fl_Double_Window(W_WINDOW,H_WINDOW,"bfide");
    Fl_Pack *packWindow = new Fl_Pack(0,0,W_WINDOW,H_WINDOW);
      Fl_Pack *packEditor = new Fl_Pack(0,0,W_WINDOW,H_EDITOR);
        Fl_Text_Editor *editor = new Fl_Text_Editor(0,0,W_EDITOR,0);
        Fl_Pack *packButtons = new Fl_Pack(0,0,W_BUTTON,0);
          Fl_Pack *packRun = new Fl_Pack(0,0,0,H_BUTTON);
            Fl_Button *buttonRevRun = new Fl_Button(0,0,W_BUTTON/2,0,"@|<");
            Fl_Button *buttonRun = new Fl_Button(0,0,W_BUTTON/2,0,"@>|");
          packRun->end();
          Fl_Pack *packStep = new Fl_Pack(0,0,0,H_BUTTON);
            Fl_Repeat_Button *buttonBack = new Fl_Repeat_Button(0,0,W_BUTTON/2,0,"@<");
            Fl_Repeat_Button *buttonForward = new Fl_Repeat_Button(0,0,W_BUTTON/2,0,"@>");
          packStep->end();
          Fl_Input *inpIo = new Fl_Input(0,0,0,H_BUTTON);
          Fl_Box *boxPromptLabel = new Fl_Box(0,0,0,H_BUTTON,"More input:");
          Fl_Pack *packPrompt = new Fl_Pack(0,0,0,H_BUTTON);
            Fl_Radio_Button *buttonPrompt = new Fl_Radio_Button(0,0,W_BUTTON/2,0,"User");
            Fl_Radio_Button *buttonNull = new Fl_Radio_Button(0,0,W_BUTTON/2,0,"Null");
          packPrompt->end();
          Fl_Button *buttonImport = new Fl_Button(0,0,0,H_BUTTON,"Open");
          Fl_Button *buttonExport = new Fl_Button(0,0,0,H_BUTTON,"Save");
        packButtons->end();
      packEditor->end();
      Fl_Text_Display *dispIo = new Fl_Text_Display(0,0,0,H_DISP);
      Fl_Scroll *scrollTape = new Fl_Scroll(0,0,W_WINDOW,H_TAPE);
        Fl_Pack *packTape = new Fl_Pack(scrollTape->x(),scrollTape->y(),1,H_CELL);
        packTape->end();
      scrollTape->end();
    packWindow->end();
  window->end();

  /* configure widgets and resize behaviour */
  packPrompt->type(Fl_Pack::HORIZONTAL);
  packStep->type(Fl_Pack::HORIZONTAL);
  packRun->type(Fl_Pack::HORIZONTAL);
  packButtons->type(Fl_Pack::VERTICAL);
  packEditor->type(Fl_Pack::HORIZONTAL);
  packTape->type(Fl_Pack::HORIZONTAL);
  packWindow->type(Fl_Pack::VERTICAL);
  scrollTape->resizable(0);
  packEditor->resizable(editor);
  packWindow->resizable(packEditor);
  window->resizable(packWindow);
  window->size_range(MIN_W_EDITOR+W_BUTTON,packButtons->children()*H_BUTTON + H_DISP + H_TAPE);

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

  /* set up state object and callbacks */
  IdeState state(H_CELL_FIELD,W_CELL,editor,dispIo,inpIo,scrollTape,packTape,openfile);
  buttonRun->callback(&run_fwd_cb, &state);
  buttonRevRun->callback(&run_back_cb, &state);
  buttonForward->callback(&step_fwd_cb, &state);
  buttonBack->callback(&step_back_cb, &state);
  buttonPrompt->callback(&prompt_cb, &state);
  buttonPrompt->setonly();
  buttonNull->callback(&null_cb, &state);
  buttonExport->callback(&export_cb,&state);
  buttonImport->callback(&import_cb,&state);
  buffProg->add_modify_callback(&edited_cb, &state);
  inpIo->callback(&inp_edited_cb, &state);
  inpIo->when(FL_WHEN_CHANGED);

  /* run */
  window->show(argc, argv);
  return Fl::run();
}
