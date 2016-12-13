#pragma once
#include <stdexcept>
#include <exception>
#include <string>

#include <vector>

/** \namespace args
 * \brief contains all the functionality of the args library
 */
namespace args
{

  class Error : public std::runtime_error
  {
  public:
    Error(const std::string &problem) : std::runtime_error(problem) {}
    virtual ~Error() {};
  };

  class Help : public Error
  {
  public:
    Help(const std::string &flag) : Error(flag) {}
    virtual ~Help() {};
  };

  class ParseError : public Error
  {
  public:
    ParseError(const std::string &flag) : Error(flag) {}
    virtual ~ParseError() {};
  };

  class Observer
  {
  public:
    virtual void update(int value) = 0;
    virtual void show(std::ostream &os) = 0;
  };

  class ArgumentParser
  {
  private:
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
    void parseArgs(const std::string &args);
    void parseArgs(const std::vector<std::string> &args);
    void parseArgs(int argc, char **argv); // @TODO: change signature to (const int argc, const char *const * argv)
  };

  struct FlagId{
    const bool isShort;
    const char shortId;
    const std::string longId;
    FlagId(const std::string &flag) : isShort(false), shortId(), longId(flag) {}
    FlagId(const char *flag) : isShort(false), shortId(), longId(flag) {}
    FlagId(const char flag) : isShort(true), shortId(flag), longId() {}
  };

  class Flag : public Observer
  {
    std::string name;
    std::string description;
  public:
    Flag( ArgumentParser &p, const std::string &a_, const std::string &b_ , std::initializer_list<FlagId> flags ):
      name(a_),
      description(b_)
    {
      p.add_parameter(this);
    }
    /* virtual */void update(int v);
    /* virtual */void show(std::ostream &os);
  };

  class HelpFlag : public Flag
  {
  public:
    HelpFlag( ArgumentParser &p, const std::string &a_, const std::string &b_ , std::initializer_list<FlagId> flags ) :
      Flag(p, a_, b_, flags) {}
  };
}

std::ostream &operator<<(std::ostream &os, const args::ArgumentParser &parser);
