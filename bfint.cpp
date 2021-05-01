#include <iostream>
#include <stack>
#include "bfint.h"

BFInt::BFInt() :
  progPos(0),
  tapePos(0),
  tape(1,0)
{}

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
                  err_output("Unmatched ]",false);
                  return false;
                }
                loops[i]=parse.top();
                loops[parse.top()]=i;
                parse.pop();
    }
  }
  if(!parse.empty())
  {
    err_output("Unmatched [",false);
    return false;
  }
  this->program = program;
  return true;
}

void BFInt::reset_exec()
{
  tape.assign(1,0);
  progPos = tapePos = 0;
  d_clear_tape();
}

signed char BFInt::step()
{
  if(progPos >= program.size())
    return 1;
  signed char status = 0;
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
    case ',': write_cell(input()); break;
    case '.': output(tape[tapePos]); break;
    case ']': write_prog_pos(loops[progPos]); return 0;
    case '[': if(!tape[tapePos])
              {
                write_prog_pos(loops[progPos]+1);
                return 0;
              }
              break;
    default : status = d_handle();
              if(status<0)
              {
                status=0;
                err_output(std::string("Unrecognized command '")+program[progPos]+'\'',true);
              }
  }
  write_prog_pos(progPos+1);
  return status>0 ? 1 : 0;
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
  d_write_tape_pos(oldPos);
}

void BFInt::write_prog_pos(unsigned newPos)
{
  unsigned oldPos = progPos;
  progPos=newPos;
  d_write_prog_pos(oldPos);
}
