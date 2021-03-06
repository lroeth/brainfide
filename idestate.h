#ifndef IDESTATE_H
#define IDESTATE_H

#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Widget.H>
#include "bfintbidir.h"

struct CellConfig
{
  int h_cell_field;
  int w_cell;
  CellConfig(int h_cell_field, int w_cell);
};

class IdeState : public BFIntBidir
{
  public:
    IdeState(int h_cell_field, int w_cell, Fl_Window *window, Fl_Text_Editor *editor, Fl_Text_Display *dispIo, Fl_Input *inpIo, Fl_Scroll *scrollTape,Fl_Pack *packTape,Fl_Menu_Item *menuSave,const char *openfile);
    ~IdeState();
    void mark_dirty();
    /* Set behavior on running out of input:
    *    prompt | prompt user for more input
    *    null   | send null character ('\0')
    *  Last called is the current setting, defaults to prompt
    */
    void prompt();
    void null();
    /* save/load script to/from file
    *  If filename is null, run file picker to get file
    *  save() overwrites currently open filename automatically,
    *    or calls file picker if no currently open file.
    */
    void new_file();
    void save_as(const char *filename=0);
    void save();
    void open(const char *filename=0);
    void close();

  private:
    bool overwrite(); /* confirm before losing unsaved changes, if necessary */
    void update_title(); /* handle window title */
    void mark_dirty_file(bool isDirty);
    void create_chooser(const char *filter, int type, const char *label); /* make sure chooser exists and has the arg attributes, then show it */

    /* handle tape pointer, for d_write_tape_pos */
    void highlight_cell(unsigned cell);
    void unhighlight_cell(unsigned cell);

    /* I/O virtual functions from bfint */
    unsigned char input();
    void backinput();
    bool input_ready();
    void output(unsigned char out);
    void backoutput();
    void err_output(std::string message, bool is_warning);
    std::string new_program();

    /* UI update virtual functions from bfint*/
    void d_block();
    int d_handle();
    bool d_backhandle();
    void d_add_cell();
    void d_write_cell(unsigned char val);
    void d_write_tape_pos(unsigned oldPos);
    void d_write_prog_pos(unsigned oldPos);
    void d_reset_exec();

    /* callback for File_Chooser */
    static void chooser_cb(Fl_File_Chooser *w, void *p);

    /* UI components needed by member functions */
    Fl_Window *window;
    Fl_Text_Editor *editor;
    Fl_Text_Display *dispIo;
    Fl_Input *inpIo;
    Fl_Scroll *scrollTape;
    Fl_Pack *packTape;
    Fl_File_Chooser *chooser;
    Fl_Menu_Item *menuSave;

    /* State data */
    const struct CellConfig config; /* information needed to construct cells */
    bool isDirtyFile; /* program has been edited since last save */
    bool isInput; /* output is ready for input */
    bool isPrompt; /* if input runs out, prompt user. if false, just feed nulls */
    const char *openfile; /* current file, or null for untitled */
};

#endif //IDESTATE_H
