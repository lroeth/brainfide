#include "bfintbidir.h"

void BFIntBidir::reset_exec()
{
  loopReps.clear();
  while(!overwrite.empty())
    overwrite.pop();
  BFInt::reset_exec();
}

int BFIntBidir::step()
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

bool BFIntBidir::backstep()
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

void BFIntBidir::write_cell_inp(unsigned char val)
{
  /* stores original value, since otherwise input is non-reversible */
  overwrite.push(tape[tapePos]);
  write_cell(val);
}

void BFIntBidir::unwrite_cell_inp()
{
  write_cell(overwrite.top());
  overwrite.pop();
}

bool BFIntBidir::step_loop(unsigned left, unsigned right, bool isLeft)
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

void BFIntBidir::backstep_loop(unsigned left, unsigned right)
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
