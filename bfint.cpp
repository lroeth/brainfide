#include <stack>
#include <stdio.h>
#include "bfint.h"

BFInt::BFInt() :
  progPos(0),
  tapePos(0),
  tape(1,0),
  isDirty(false),
  isRun(false),
  isBlocking(false)
{}

void BFInt::reset_exec()
{
  tape.assign(1,0);
  write_tape_pos(0);
  write_prog_pos(0);
  d_reset_exec();
}

bool BFInt::step_fwd()
{
  if(clean()<0)
    return false;
  isRun=false;
  step();
  return true;
}

bool BFInt::run_fwd()
{
  if(clean()<0)
    return false;
  isRun=true;
  while(!step());
  return true;
}

void BFInt::unblock()
{
  if(!isBlocking)
    return;
  isBlocking=false;
  if(isRun)
    run_fwd();
  else
    step_fwd();
}

void BFInt::mark_dirty()
  {isDirty=true;}



std::string BFInt::export_c(std::string program)
{
  std::string out = "#include<stdio.h>\n"
                    "#include<stdlib.h>\n"
                    "int main(){\n"
                      "unsigned size = 1024;\n"
                      "unsigned init = 1024;\n"
                      "unsigned char *tape = calloc(size,1);\n"
                      "unsigned char *t=tape;\n"
                      "int b;\n";
  for(int i=0;i<program.size();i++)
  {
    switch(program[i])
    {
      case '<': out+="if(t-- == tape) return 1;"; break;
      case '>': out+="if(++t-tape == size){\n"
                       "tape = realloc(tape,size*2);\n"
                       "t = tape + size;\n"
                       "size *= 2;}\n"
                     "if(t-tape == init){\n"
                       "*t=0;\n"
                       "init++;}\n"; break;
      case '+': out+="++*t;\n"; break;
      case '-': out+="--*t;\n"; break;
      case ',': out+="b=fgetc(stdin);*t=b==(-1)?'\\0':b;\n"; break;
      case '.': out+="putchar(*t);\n"; break;
      case ']': out+="}\n"; break;
      case '[': out+="while(*t!=0){\n"; break;
    }
  }
  out+="free(tape);\nreturn 0;}\n";
  return out;
}



int BFInt::step()
{
  if(progPos >= program.size())
    return 1;
  int status = 0;
  switch(program[progPos])
  {
    case '<': if(!tapePos)
              {
                err_output("Can't go left at cell 0",false);
                return -1;
              }
              write_tape_pos(tapePos - 1);
              break;
    case '>': write_tape_pos(tapePos + 1);
              break;
    case '+': write_cell(tape[tapePos]+1); break;
    case '-': write_cell(tape[tapePos]-1); break;
    case ',': if(!input_ready())
              {
                block();
                return 2;
              }
              write_cell(input()); break;
    case '.': output(tape[tapePos]); break;
    case ']': if(!tape[tapePos]) break;
              write_prog_pos(loops[progPos]+1); return 0;
    case '[': if(tape[tapePos]) break;
              write_prog_pos(loops[progPos]+1); return 0;
    default : status = d_handle();
              if(status<0)
              {
                status=0;
                err_output(std::string("Unrecognized command '")+program[progPos]+'\'',true);
              }
  }
  write_prog_pos(progPos+1);
  return status;
}

void BFInt::block()
{
  isBlocking = true;
  d_block();
}

int BFInt::clean()
{
  if(!isDirty)
    return 0;
  if(!update_program(new_program()))
    return -1;
  reset_exec();
  isDirty = false;
  return 1;
}

bool BFInt::update_program(std::string program)
{
  std::stack<unsigned> parse;
  for(int i=0;i<program.size();i++)
  {
    switch(program[i])
    {
      case '[': parse.push(i); break;
      case ']': if(parse.empty())
                {
                  char buff[20];
                  snprintf(buff,20,"Unmatched ] at %i",i);
                  write_prog_pos(i);
                  err_output(buff,false);
                  return false;
                }
                loops[i]=parse.top();
                loops[parse.top()]=i;
                parse.pop();
    }
  }
  if(!parse.empty())
  {
    char buff[20];
    snprintf(buff,20,"Unmatched [ at %i",parse.top());
    write_prog_pos(parse.top());
    err_output(buff,false);
    return false;
  }
  this->program = program;
  return true;
}

unsigned BFInt::get_prog_pos()
  {return progPos;}

unsigned BFInt::get_tape_pos()
  {return tapePos;}

int BFInt::get_cmd(unsigned pos)
{
  if(pos>=program.size())
    return -1;
  return program[pos];
}

unsigned char BFInt::get_cell(unsigned pos)
{
  if(pos>=tape.size())
    return 0;
  return tape[pos];
}

void BFInt::write_cell(unsigned char val)
{
  tape[tapePos] = val;
  d_write_cell(val);
}

void BFInt::write_tape_pos(unsigned newPos)
{
  unsigned oldPos = tapePos;
  while(newPos>=tape.size())
  {
    tapePos = tape.size();
    tape.push_back(0);
    d_add_cell();
  }
  tapePos = newPos;
  d_write_tape_pos(oldPos);
}

void BFInt::write_prog_pos(unsigned newPos)
{
  unsigned oldPos = progPos;
  progPos=newPos;
  d_write_prog_pos(oldPos);
}
