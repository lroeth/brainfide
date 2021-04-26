#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>


int main(int argc, char **argv)
{
  Fl_Window *window = new Fl_Window(800,600);
    Fl_Group *groupResize = new Fl_Group(20,20,760,560);
      Fl_Text_Editor *editor = new Fl_Text_Editor(20,20,760,560);
    groupResize->end();
  window->end();

  Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
  editor->buffer(buffer);
  window->resizable(groupResize);

  window->show(argc, argv);
  return Fl::run();
}
