#pragma once
#include <stdexcept>
#include <exception>
#include <string>
#include <array>
#include <typeinfo>
#include <assert.h>

#include <vector>
#include <algorithm>
#include <sstream>

/** \namespace args
 * \brief contains all the functionality of the args library
 */
namespace args
{

  std::vector<std::string> string2tokens(int argc, const char *const *argv);
  class Error : public std::runtime_error
  {
  public:
    Error(const std::string &problem) : std::runtime_error(problem) {}
    virtual ~Error()=default;
  };

  class Help : public Error
  {
  public:
    Help(const std::string &flag) : Error(flag) {}
  };

  class ParseError : public Error
  {
  public:
    ParseError(const std::string &flag) : Error(flag) {}
  };

  class UnknownParameter : public Error
  {
  public:
    UnknownParameter(const std::string &flag) : Error(flag) {}
  };

  struct FlagId{
    const bool isShort;
    const char shortId;
    const std::string longId;
    FlagId(const std::string &flag) : isShort(false), shortId(), longId(flag) {}
    FlagId(const char *flag) : isShort(false), shortId(), longId(flag) {}
    FlagId(const char flag) : isShort(true), shortId(flag), longId() {}
  };

  class Observer
  {
  protected:
    virtual void _update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args ) = 0;
  public:
    Observer( std::initializer_list<FlagId> flags_ ) : flags(flags_) {}
    virtual ~Observer() = default;
    void update(std::vector<std::string> &args);
    virtual void show(std::ostream &os) = 0;
    //private:
    std::vector<FlagId> flags;
  };

  class ArgumentParser
  {
  private:
    std::string program_call;
    std::string description;
    std::string epilog;

    std::string longprefix;
    std::string shortprefix;

    std::string longseparator;

    std::vector< Observer *> _parameters;

  public:
    ArgumentParser(const std::string &description_, const std::string &epilog_ = std::string()) :
      description(description_),
      epilog(epilog_),
      longprefix("--"),
      shortprefix("-"),
      longseparator("=") {}

    void add_parameter ( Observer *p); // @TODO: make this signature (const Observer *p)


    /** Pass the help menu into an ostream
     */
    void display_help(std::ostream &help) const;

    /** Parse all arguments.
     *
     * \param args an iterable of the arguments
     * \return the iterator after the last parsed value.  Only useful for kick-out
     */
    // template <typename T> void ParseArgs(const T &args);
    void parseArgs(const std::vector<std::string> &args);
    void parseArgs(int argc, const char *const *argv);
  };



  template < typename T >
  class Parameter: public Observer
  {
  protected:
    std::string name;
    std::string description;
    T value;

  public:
    Parameter( ArgumentParser &p, const std::string &name_, const std::string &description_ ,
               std::initializer_list<FlagId> flags_, const T &default_value):
      Observer(flags_),
      name(name_),
      description(description_),
      value(default_value)
    {
      p.add_parameter(this);
    }

    /* virtual */void _update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args ) override;
    /* virtual */void show(std::ostream &os) override;
    T get();
  };

  class Flag : public Parameter<bool>
  {
  public:
    Flag( ArgumentParser &p, const std::string &name_, const std::string &description_,
          std::initializer_list<FlagId> flags_ ):
      Parameter( p, name_, description_, flags_, false){}

    Flag( ArgumentParser &p, const std::string &name_, const std::string &description_,
          std::initializer_list<FlagId> flags_ , void (*action_)(void)):
      Parameter( p, name_, description_, flags_, false), action(action_){}

    /* virtual */void _update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args ) override;
    void (*action)(void);
  };

  class HelpFlag : public Flag
  {
  public:
    HelpFlag( ArgumentParser &p, const std::string &name_, const std::string &description_ , std::initializer_list<FlagId> flags_ ) :
      Flag(p, name_, description_, flags_) {}

    /* virtual */void _update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args ) override;
  };
}

std::ostream &operator<<(std::ostream &os, const args::ArgumentParser &parser);
std::string flagId_to_string(const args::FlagId &f);


#include "argparse.tpp"
