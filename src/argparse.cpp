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

void args::ArgumentParser::parseArgs(int argc, const char *const *argv)
{
  const auto arguments = string2tokens(argc, argv);
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

namespace{
  void push_previous_parameter( std::vector<std::string> &keys,
                                std::vector<std::string> &values,
                                std::string &k, std::string &v )
  {
    if ( !k.empty() )
      {
        keys.push_back(k);
        values.push_back(v);
      }
  }

  const bool is_key (std::string s)
  {
    const auto token_position = s.find_first_not_of("-");
    return (token_position != 0);
  }

  std::vector<std::string> merge_keys_and_values(std::vector<std::string> &keys,
                                                 std::vector<std::string> &values)
  {
    std::vector<std::string> result;
    auto k_it = keys.begin();
    auto v_it = values.begin();
    int i;
    for (; k_it < keys.end(); ++k_it, ++v_it)
      {
        result.push_back(*k_it);
        result.push_back(*v_it);
      }
    return result;
  }
}

std::vector<std::string> args::string2tokens(int argc, const char *const *argv)
{

  const std::vector<std::string> arguments(&argv[1], argv + argc);
  std::vector<std::string> keys, values;
  std::string k,v;
  for (auto &a : arguments)
  {
    if ( is_key(a) )
      {
        push_previous_parameter(keys, values, k, v);
        k = a;
        v.clear();
      }
    else
      {
        v.append(" "+a);
      }
  }

  push_previous_parameter(keys, values, k, v);
  return merge_keys_and_values( keys, values);
}
