#include <string>
#include <vector>
#include <map>

class State
{
  public:
    State();
    bool update_program(std::string program);
    int step();
    void reset_exec();
  protected:
    virtual bool handle(char command);
    virtual unsigned char input();
    virtual void output(unsigned char out);
    virtual void err_output(std::string message, bool is_recoverable);

    unsigned progPos;
    unsigned tapePos;
    std::string program;
    std::vector<unsigned char> tape;
    std::map<unsigned,unsigned> loops;
};
