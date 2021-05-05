#include <algorithm>
#include <stdio.h>
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

void BFInt::reset_exec()
{
  tape.assign(1,0);
  write_tape_pos(0);
  write_prog_pos(0);
  loopReps.clear();
  while(!overwrite.empty())
    overwrite.pop();
  d_clear_tape();
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
                return 2;
              write_cell_inp(input()); break;
    case '.': output(tape[tapePos]); break;
    case ']': if(step_loop(loops[progPos],progPos,false)) return 0; break;
    case '[': if(step_loop(progPos,loops[progPos],true)) return 0; break;
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

bool BFInt::backstep()
{
  if(progPos == 0)
    return false;
  write_prog_pos(progPos-1);
  switch(program[progPos])
  {
    case '<': write_tape_pos(tapePos + 1); break;
    case '>': write_tape_pos(tapePos - 1); break;
    case '+': write_cell(tape[tapePos]-1); break;
    case '-': write_cell(tape[tapePos]+1); break;
    case ',': unwrite_cell_inp();          break;
    case '.': break;
    case ']': backstep_loop(loops[progPos],progPos); break;
    case '[': backstep_loop(progPos,loops[progPos]); break;
    default : return d_backhandle();
  }
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
  /* for changing the cell in reverse, or via plus/minus */
  tape[tapePos] = val;
  d_write_cell(val);
}

void BFInt::write_cell_inp(unsigned char val)
{
  /* stores original value, since otherwise input is non-reversible */
  overwrite.push(tape[tapePos]);
  write_cell(val);
}

void BFInt::unwrite_cell_inp()
{
  write_cell(overwrite.top());
  overwrite.pop();
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




bool BFInt::step_loop(unsigned left, unsigned right, bool isLeft)
{
  std::stack<unsigned> *curr = &loopReps[left];
  if(isLeft)
    curr->push(0);
  else
    curr->top()++;
  if(isLeft == !tape[tapePos]) /* a xor b */
  {
    write_prog_pos(isLeft ? right+1 : left+1);
    return true;
  }
  return false;
}

void BFInt::backstep_loop(unsigned left, unsigned right)
{
  std::stack<unsigned> *curr = &loopReps[left];
  if(curr->top())
  {
    curr->top()--;
    write_prog_pos(right);
    return;
  }
  curr->pop();
  write_prog_pos(left);
}
