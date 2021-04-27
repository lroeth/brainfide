#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

/* config macros */
#define W_WINDOW 800
#define H_WINDOW 600
/* W_BUTTON - W_PART_BUTTON >= 60 */
#define W_BUTTON 100
#define H_BUTTON 40
#define H_DISP 100


/* convenience macros */
/* TODO: use Fl_Packs instead of hand positioning */
#define W_EDITOR W_WINDOW - W_BUTTON
#define H_EDITOR H_WINDOW - H_DISP
#define W_DISP W_WINDOW
#define NUM_BUTTONS 2

int main(int argc, char **argv)
{
  /* widget hierarchy */
  Fl_Window *window = new Fl_Window(W_WINDOW,H_WINDOW);
    Fl_Group *groupResize = new Fl_Group(0,0,W_EDITOR,H_EDITOR);
      Fl_Text_Editor *editor = new Fl_Text_Editor(0,0,W_EDITOR,H_EDITOR);
    groupResize->end();
    Fl_Group *groupButtons = new Fl_Group(W_EDITOR,0,W_BUTTON,H_EDITOR);
      Fl_Button *buttonRun = new Fl_Button(W_EDITOR,0,W_BUTTON,H_BUTTON,"Run");
      Fl_Button *buttonBack = new Fl_Button(W_EDITOR,1 * H_BUTTON,W_BUTTON/2,H_BUTTON,"@<");
      Fl_Button *buttonForward = new Fl_Button(W_EDITOR + W_BUTTON/2,1 * H_BUTTON,W_BUTTON/2,H_BUTTON,"@>");
      Fl_Box *spaceButtons = new Fl_Box(W_EDITOR,NUM_BUTTONS * H_BUTTON,W_BUTTON,H_EDITOR - NUM_BUTTONS * H_BUTTON);
    groupButtons->end();
    Fl_Text_Display *dispIo = new Fl_Text_Display(0,H_EDITOR,W_DISP,H_DISP);
  window->end();

  /* adjust widgets */
  window->resizable(groupResize);
  groupButtons->resizable(spaceButtons);

  /* setup data structures and callbacks */
  Fl_Text_Buffer *bufferProg = new Fl_Text_Buffer();
  editor->buffer(bufferProg);
  Fl_Text_Buffer *bufferIo = new Fl_Text_Buffer();
  dispIo->buffer(bufferIo);
  bufferIo->append(">");

  /* run */
  window->show(argc, argv);
  return Fl::run();
}
