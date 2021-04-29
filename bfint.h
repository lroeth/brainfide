#include <string>
#include <vector>
#include <map>

class State
{
  public:
    State(unsigned tapeSize);
    void update_program(std::string program);
    bool step();
    void reset_exec();
  protected:
    virtual bool handle(char command);
    virtual unsigned char input();
    virtual void output(unsigned char out);
    virtual void err_output(std::string message, bool is_fatal);

    unsigned progPos;
    unsigned tapePos;
    const unsigned tapeSize;
    std::string program;
    std::vector<unsigned char> tape;
    std::map<unsigned,unsigned> loops;
};
