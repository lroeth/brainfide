#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>

/* config macros */
#define W_WINDOW 800
#define H_WINDOW 600
#define W_BUTTON 100
#define H_BUTTON 40
#define H_DISP 100
#define H_CELL_FIELD 20
#define W_CELL 80
#define MIN_W_EDITOR 100

/* convenience macros */
#define H_CELL 3*H_CELL_FIELD
#define H_TAPE (H_CELL + Fl::scrollbar_size())
#define W_EDITOR W_WINDOW - W_BUTTON
#define H_EDITOR H_WINDOW - H_DISP - H_TAPE


int main(int argc, char **argv)
{
  /* widget hierarchy */
  Fl_Window *window = new Fl_Window(W_WINDOW,H_WINDOW,"bfide");
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
      Fl_Text_Display *dispIo = new Fl_Text_Display(0,0,0,H_DISP);
      Fl_Scroll *scrollTape = new Fl_Scroll(0,0,0,H_TAPE);
        Fl_Pack *packTape = new Fl_Pack(scrollTape->x(),scrollTape->y(),1,H_CELL);
          Fl_Group *cell0 = new Fl_Group(0,0,W_CELL,H_CELL);
            Fl_Output *cell0char = new Fl_Output(0,0,W_CELL,H_CELL_FIELD);
            Fl_Output *cell0val = new Fl_Output(0,H_CELL_FIELD,W_CELL,H_CELL_FIELD);
            Fl_Box *cell0num = new Fl_Box(0,2*H_CELL_FIELD,W_CELL,H_CELL_FIELD,"0");
          cell0->end();
          Fl_Group *cell1 = new Fl_Group(0,0,W_CELL,H_CELL);
            Fl_Output *cell1char = new Fl_Output(0,0,W_CELL,H_CELL_FIELD);
            Fl_Output *cell1val = new Fl_Output(0,H_CELL_FIELD,W_CELL,H_CELL_FIELD);
            Fl_Box *cell1num = new Fl_Box(0,2*H_CELL_FIELD,W_CELL,H_CELL_FIELD,"1");
          cell1->end();
          Fl_Group *cell2 = new Fl_Group(0,0,W_CELL,H_CELL);
            Fl_Output *cell2char = new Fl_Output(0,0,W_CELL,H_CELL_FIELD);
            Fl_Output *cell2val = new Fl_Output(0,H_CELL_FIELD,W_CELL,H_CELL_FIELD);
            Fl_Box *cell2num = new Fl_Box(0,2*H_CELL_FIELD,W_CELL,H_CELL_FIELD,"2");
          cell2->end();
          Fl_Group *cell3 = new Fl_Group(0,0,W_CELL,H_CELL);
            Fl_Output *cell3char = new Fl_Output(0,0,W_CELL,H_CELL_FIELD);
            Fl_Output *cell3val = new Fl_Output(0,H_CELL_FIELD,W_CELL,H_CELL_FIELD);
            Fl_Box *cell3num = new Fl_Box(0,2*H_CELL_FIELD,W_CELL,H_CELL_FIELD,"3");
          cell3->end();
        packTape->end();
      scrollTape->end();
    packWindow->end();
  window->end();

  /* configure widgets and resize behaviour */
  packStep->type(Fl_Pack::HORIZONTAL);
  packButtons->type(Fl_Pack::VERTICAL);
  packEditor->type(Fl_Pack::HORIZONTAL);
  packTape->type(Fl_Pack::HORIZONTAL);
  packWindow->type(Fl_Pack::VERTICAL);
  scrollTape->resizable(0);
  packEditor->resizable(editor);
  packWindow->resizable(packEditor);
  window->resizable(packWindow);
  window->size_range(MIN_W_EDITOR+W_BUTTON,packButtons->children()*H_BUTTON + H_DISP + H_TAPE);

  /* setup data structures and callbacks */
  cell0char->value("W");
  cell1char->value("X");
  cell2char->value("Y");
  cell3char->value("Z");
  cell0val->value("57");
  cell1val->value("58");
  cell2val->value("59");
  cell3val->value("5A");
  Fl_Text_Buffer *bufferProg = new Fl_Text_Buffer();
  editor->buffer(bufferProg);
  Fl_Text_Buffer *bufferIo = new Fl_Text_Buffer();
  dispIo->buffer(bufferIo);
  bufferIo->append(">");

  /* run */
  window->show(argc, argv);
  return Fl::run();
}
