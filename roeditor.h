#include <FL/Fl_Text_Display.H>
#include <queue>

class RO_Editor : public Fl_Text_Display
{
  public:
    RO_Editor(int X, int Y, int W, int H, const char* label=0) :
      Fl_Text_Display(X,Y,W,H,label),
      getchar_fp(0),
      isBlocked(false),
      p(0)
      {}
    void set_getchar(char (*getchar_fp)(void *), void *p);
    bool is_blocked();
    char getchar();
  private:
    char (*getchar_fp)(void *p);
    bool isBlocked;
    void *p;
};
