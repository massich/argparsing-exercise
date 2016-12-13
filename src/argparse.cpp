#include <sstream>
#include <iostream>
#include "argparse.h"

// template <typename T> void args::ArgumentParser::ParseArgs(const T &args)
void args::ArgumentParser::parseArgs(const std::string &args)
{
  std::cout << "ParseArgs string" << std::endl;
  throw Help("parseArgs string error");
}
void args::ArgumentParser::parseArgs(const std::vector<std::string> &args)
{
  std::cout << "ParseArgs vector" << std::endl;
  int i=0;
  for ( auto &s : args) {
    std::cout << i++ << "  " << s << std::endl;
  }
}


void args::ArgumentParser::add_parameter ( Observer *p)
{
  this->_parameters.push_back(p);
}

void args::ArgumentParser::parseArgs(int argc, char **argv)
{
  const std::vector<std::string> arguments(argv, argv + argc);
  this->parseArgs(arguments);
}

void args::ArgumentParser::display_help(std::ostream &help) const
{
  bool hasoptions = false;
  bool hasarguments = false;
  struct{
    unsigned int descriptionindent = 2;
  }helpParams;

  std::vector<std::string> description_text {};
  description_text.push_back(this->description);

  std::vector<std::string> epilog_text {};
  epilog_text.push_back(this->epilog);

  help << '\n';

  for (const auto &line: description_text)
    {
      help << std::string(helpParams.descriptionindent, ' ') << line << "\n";
    }
  help << "\n";

  for (const auto p: _parameters)
  {
    p->show(help);
    help << "\n";
  }

  for (const auto &line: epilog_text)
    {
      help << std::string(helpParams.descriptionindent, ' ') << line << "\n";
    }
  help << "\n";
}

std::ostream &operator<<(std::ostream &os, const args::ArgumentParser &parser)
{
  parser.display_help(os);
  return os;
}

void args::Flag::update(int v)
{
  std::cout << "this is Flag update observer" << std::endl;
}

void args::Flag::show(std::ostream &os)
{
  os << "A:" << this->name << " B:" << this->description ;
}
