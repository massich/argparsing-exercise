#include <sstream>
#include <iostream>
#include <algorithm>
#include "argparse.h"

// template <typename T> void args::ArgumentParser::ParseArgs(const T &args)
void args::ArgumentParser::parseArgs(const std::string &args)
{
  std::cout << "ParseArgs string" << std::endl;
  throw Help("parseArgs string error");
}

void args::ArgumentParser::parseArgs(const std::vector<std::string> &args)
{
  std::vector<std::string> consumible_args(args);
  for (const auto p: _parameters)
    {
      p->update(consumible_args);
    }
  int i=0;
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

std::string flagId_to_string(const args::FlagId &f)
{
  std::string out("-");
  if (f.isShort)
    out.append(1, f.shortId);
  else
    {
      out.append("-"+f.longId);
    }
  // @TODO return (f.isShort) ? "-"+f.shortId : "--"+f.longId;
  return out;
}

bool args::Flag::get()
{
  return this->value;
}

void args::Flag::update(std::vector<std::string> &args)
{
  for (auto &f: this->flags)
  {
    auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
    if ( it != args.end() )
    {
      this->value = true;
      it = args.erase(it);
      break;
    }
  }
}

void args::Flag::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}

void args::ParameterInt::update(std::vector<std::string> &args)
{
  for (auto &f: this->flags)
    {
      auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
      if ( it != args.end() )
        {
          auto element_it = it;
          std::stringstream(*(++element_it)) >> this->value;
          element_it = args.erase(element_it);
          it = args.erase(it);
          break;
        }
    }
}

void args::ParameterChar::update(std::vector<std::string> &args)
{
  for (auto &f: this->flags)
    {
      auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
      if ( it != args.end() )
        {
          auto element_it = it;
          std::stringstream(*(++element_it)) >> this->value;
          element_it = args.erase(element_it);
          it = args.erase(it);
          break;
        }
    }
}

void args::ParameterStr::update(std::vector<std::string> &args)
{
  for (auto &f: this->flags)
    {
      auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
      if ( it != args.end() )
        {
          auto element_it = it;
          std::stringstream(*(++element_it)) >> this->value;
          element_it = args.erase(element_it);
          it = args.erase(it);
          break;
        }
    }
}

/* TODO this is code duplicated between Flag and Parameter, so a class where
 * to inhered both should be crated.
 *
 */
int args::ParameterInt::get()
{
  return this->value;
}


void args::ParameterInt::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}

char args::ParameterChar::get()
{
  return this->value;
}


void args::ParameterChar::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}

std::string args::ParameterStr::get()
{
  return this->value;
}


void args::ParameterStr::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}
