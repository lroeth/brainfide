#include <FL/Fl_Text_Display.H>
#include <queue>

class RO_Editor : public Fl_Text_Display
{
  public:
    RO_Editor(int X, int Y, int W, int H, const char* label=0) :
      Fl_Text_Display(X,Y,W,H,label),
      handle_fp(&RO_Editor::kb_handle),
      getchar_fp(&RO_Editor::q_getchar),
      isBlocked(false),
      p(this),
      inpQ()
      {}
    void set_getchar(char (*getchar_fp)(void *), void *p);
    bool is_blocked();
    char getchar();
    int handle(int e);
  private:
    static int kb_handle(RO_Editor *t,int e);
    static int default_handle(RO_Editor *t,int e);
    static char q_getchar(void *p);

    int (*handle_fp)(RO_Editor *t,int e);
    char (*getchar_fp)(void *p);
    bool isBlocked;
    void *p;
    std::queue<char> inpQ;
};
