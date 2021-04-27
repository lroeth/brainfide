#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

/* config macros */
#define W_WINDOW 800
#define H_WINDOW 600
#define W_BUTTON 100
#define H_BUTTON 40
#define H_DISP 100
#define MIN_W_EDITOR 100

/* convenience macros */
#define W_EDITOR W_WINDOW - W_BUTTON
#define H_EDITOR H_WINDOW - H_DISP
#define W_DISP W_WINDOW

int main(int argc, char **argv)
{
  /* widget hierarchy */
  Fl_Window *window = new Fl_Window(W_WINDOW,H_WINDOW);
    Fl_Pack *packWindow = new Fl_Pack(0,0,W_WINDOW,H_WINDOW);
      Fl_Pack *packEditor = new Fl_Pack(0,0,W_WINDOW,H_EDITOR);
        Fl_Text_Editor *editor = new Fl_Text_Editor(0,0,W_EDITOR,0);
        Fl_Pack *packButtons = new Fl_Pack(0,0,W_BUTTON,0);
          Fl_Button *buttonRun = new Fl_Button(0,0,0,H_BUTTON,"Run");
          Fl_Pack *packStep = new Fl_Pack(0,0,0,H_BUTTON);
            Fl_Button *buttonBack = new Fl_Button(0,0,W_BUTTON/2,0,"@<");
            Fl_Button *buttonForward = new Fl_Button(0,0,W_BUTTON/2,0,"@>");
          packStep->end();
        packButtons->end();
      packEditor->end();
      Fl_Text_Display *dispIo = new Fl_Text_Display(0,H_EDITOR,W_DISP,H_DISP);
    packWindow->end();
  window->end();

  /* configure widgets and resize behaviour */
  packWindow->type(Fl_Pack::VERTICAL);
  packEditor->type(Fl_Pack::HORIZONTAL);
  packButtons->type(Fl_Pack::VERTICAL);
  packStep->type(Fl_Pack::HORIZONTAL);
  packEditor->resizable(editor);
  packWindow->resizable(packEditor);
  window->resizable(packWindow);
  window->size_range(MIN_W_EDITOR+W_BUTTON,packButtons->children()*H_BUTTON + H_DISP);

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
