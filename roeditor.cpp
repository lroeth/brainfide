#include <FL/Fl.H>
#include "roeditor.h"

void RO_Editor::set_getchar(char (*getchar_fp)(RO_Editor *,void *), void *p)
{
  this->getchar_fp = getchar_fp;
  this->p = p;
  handle_fp = &default_handle;
}


bool RO_Editor::is_blocked()
  {return isBlocked;}


char RO_Editor::getchar()
{
  char out;
  isBlocked=true;
  out = getchar_fp(this, p);
  isBlocked=false;
  if(buffer())
  {
    char buff[2];
    buff[0]=out;
    buff[1]='\0';
    buffer()->append(buff);
  }
  return out;
}


int RO_Editor::handle(int e)
  {return handle_fp(this,e);}




int RO_Editor::kb_handle(RO_Editor *t,int e)
{
  if(t->isBlocked && e==FL_KEYBOARD)
  {
    int key = Fl::event_key();
    {
      for(int i=0;i<Fl::event_length();i++)
        t->inpQ.push(Fl::event_text()[i]);
      return 1;
    }
  }
  return RO_Editor::default_handle(t,e);
}


int RO_Editor::default_handle(RO_Editor *t, int e)
  {return t->Fl_Text_Display::handle(e);}


char RO_Editor::q_getchar(RO_Editor *t, void *p)
{
  while(t->inpQ.empty())
    Fl::wait();
  char out = t->inpQ.front();
  t->inpQ.pop();
  return out;
}
