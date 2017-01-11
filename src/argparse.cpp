#include <sstream>
#include <iostream>
#include <algorithm>
#include "argparse.h"

std::string get_and_eliminate_program_call( std::vector<std::string> &consumible_args )
{
  assert(!consumible_args.empty());
  std::string program_call = *consumible_args.begin();
  consumible_args.erase(consumible_args.begin());
  return program_call;
}

namespace{
  std::string get_unkParameter_message( const std::vector<std::string> &args)
  {
    std::ostringstream oss;
    oss << args.size() << " unknown parameters:";
    for (const auto &a : args) oss << " " << a << ",";
    oss << '\n';
    return oss.str();
  }
}


void args::ArgumentParser::parseArgs(const std::vector<std::string> &args)
{
  std::vector<std::string> consumible_args(args);
  program_call = get_and_eliminate_program_call( consumible_args );
  for (const auto p: _parameters)
    {
      p->update(consumible_args);
    }
  if (!consumible_args.empty())
    {
      throw UnknownParameter( get_unkParameter_message(consumible_args) );
    }
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

void args::ArgumentParser::process()
{
  for ( const auto p: _parameters)
    p->process();
}

void args::ArgumentParser::processArgs(int argc, const char *const *argv)
{
  this->parseArgs( argc, argv );
  this->process();
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
  return (f.isShort) ? std::string("-").append(1,f.shortId) : "--"+f.longId;
}

void args::Observer::update(std::vector<std::string> &args)
{
  for (auto &f: this->flags)
    {
      auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
      if ( it != args.end() )
        {
          matched = true;
          _update_and_consume_if_necessary( it, args );
          it = args.erase(it);
          break;
        }
    }
}

void args::Observer::process()
{
  if ( matched && action ) { action(); }
}

void args::Flag::_update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args )
{
  args.erase(++it); // string2tokens adds "" for flags with no argument
  this->value = true;
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
    assert(keys.size() == values.size());
    std::vector<std::string> result;
    auto k_it = keys.begin();
    auto v_it = values.begin();
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
        (v.empty()) ? v.append(a) : v.append(" "+a);
      }
  }
  push_previous_parameter(keys, values, k, v);

  auto result = merge_keys_and_values( keys, values);
  result.insert(result.begin(), std::string(argv[0]));

  return result;
}

template<>
void args::Parameter<std::string>::_update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args)
{
  this->value = std::string(*(++it));
  args.erase(it);
}
