#include <iostream>
#include <stack>
#include "bfint.h"

State::State() :
  program("")
{
  reset_exec();
}

bool State::update_program(std::string program)
{
  std::stack<unsigned> parse;
  for(int i=0;i<program.size();i++)
  {
    switch(program[i])
    {
      case '[': parse.push(i); break;
      case ']': if(parse.empty()) goto unmatched;
                loops[i]=parse.top();
                loops[parse.top()]=i;
                parse.pop();
    }
  }
  if(!parse.empty()) goto unmatched;
  this->program = program;
  return true;
  unmatched: err_output("Unmatched [ or ]",false); return false;
}

int State::step()
{
  if(progPos >= program.size())
    return 1;
  bool handled = handle(program[progPos]);
  progPos++;
  return handled ? 0 : -1;
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
                err_output("Can't go left at cell 0",false);
                return false;
              } 
              tapePos--; break;
    case '>': if(++tapePos==tape.size())
                tape.push_back(0);
              break;
    case '+': tape[tapePos]++; break;
    case '-': tape[tapePos]--; break;
    case ',': tape[tapePos] = input(); break;
    case '.': output(tape[tapePos]); break;
    case ']': progPos = loops[progPos]; /* no break */
    case '[': if(!tape[tapePos])
                progPos = loops[progPos];
              break;
    case ' ':
    case '\t':
    case '\n': break;
    default : err_output(std::string("Unrecognized command '") + command + '\'',true);
  }
  return true;
}

unsigned char State::input()
{
  char in = std::cin.get();
  if(!std::cin.good())
  {
    err_output("EOF on input",true);
    in='\0';
  }
  return in;
}

void State::output(unsigned char out)
  {std::cout<<out;}

void State::err_output(std::string message, bool is_recoverable)
  {std::cerr<<(is_recoverable ? "ERROR: ":"WARNING: ")<<message<<std::endl;}
