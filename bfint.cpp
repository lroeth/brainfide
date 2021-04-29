#include <iostream>
#include "bfint.h"

State::State(unsigned tapeSize) :
  tapeSize(tapeSize),
  program("")
{
  reset_exec();
}

void State::update_program(std::string program)
{
  this->program = program;
  /*! parse parentheses */
}

bool State::step()
{
  if(progPos >= program.size())
  {
    reset_exec();
    return true;
  }
  bool handled = handle(program[progPos]);
  progPos++;
  return handled;
}

void State::reset_exec()
{
  tape.resize(1);
  tape[0] = progPos = tapePos = 0;
}

bool State::handle(char command)
{
  switch(command)
  {
    case '<': if(!tapePos)
              {
                err_output("Can't go left at cell 0",true);
                return true;
              } 
              tapePos--; break;
    case '>': if(tapePos==(tape.size()-1))
                tapePos++;
              else
                tapePos=0;
              break;
    case '+': tape[tapePos]++; break;
    case '-': tape[tapePos]--; break;
    case ',': tape[tapePos] = input(); break;
    case '.': output(tape[tapePos]); break;
    case ']': progPos = loops[progPos]; /* no break */
    case '[': if(!tape[tapePos])
                progPos = loops[progPos];
              break;
    default : err_output(std::string("Unrecognized command '") + command + '\'',false);
              return false;
  }
  return false;
}

unsigned char input()
{
  int in = std::cin.get();
  if(in==EOF)
  {
    err_output("EOF on input",false);
    in='\0';
  }
  return in;
}

void output(unsigned char out)
  {std::cout<<out;}

void err_output(std::string message, bool is_fatal)
  {std::cerr<<(is_fatal ? "ERROR: ":"WARNING: ")<<message<<std::endl;}
