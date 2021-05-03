#include <FL/Fl.H>
#include "roeditor.h"

void RO_Editor::set_getchar(char (*getchar_fp)(void *), void *p)
{
  this->getchar_fp = getchar_fp;
  this->p = p;
}


bool RO_Editor::is_blocked()
  {return isBlocked;}


char RO_Editor::getchar()
{
  char out;
  isBlocked=true;
  out = getchar_fp(p);
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
