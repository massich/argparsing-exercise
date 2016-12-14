#pragma once
#include <stdexcept>
#include <exception>
#include <string>

#include <vector>
#include <algorithm>
#include <sstream>

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
    virtual void update(std::vector<std::string> &args) = 0;
    virtual void show(std::ostream &os) = 0;
    // @TODO: the getter should be here but then I don't know how to template the output
    // auto get() = 0; ?????
    // template <typename Parameter>
    // virtual Parameter get() = 0;
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
    std::vector<FlagId> flags;
    bool value;

  public:
    Flag( ArgumentParser &p, const std::string &name_, const std::string &description_ , std::initializer_list<FlagId> flags_ ):
      name(name_),
      description(description_),
      flags(flags_),
      value(false)
    {
      p.add_parameter(this);
    }
    /* virtual */void update(std::vector<std::string> &args);
    /* virtual */void show(std::ostream &os);
    bool get();
  };

  class HelpFlag : public Flag
  {
  public:
    HelpFlag( ArgumentParser &p, const std::string &name_, const std::string &description_ , std::initializer_list<FlagId> flags_ ) :
      Flag(p, name_, description_, flags_) {}
  };

  // template <typename Parameter_type>
  // using Parameter_type = typename int;
  class Parameter: public Observer
  {
  protected:
    std::string name;
    std::string description;
    std::vector<FlagId> flags;

  public:
    Parameter( ArgumentParser &p, const std::string &name_, const std::string &description_ ,
               std::initializer_list<FlagId> flags_):
      name(name_),
      description(description_),
      flags(flags_)
    {
      p.add_parameter(this);
    }
  };

  template < typename T >
  class ParameterXX : public Parameter
  {
    T value;

  public:
    ParameterXX( ArgumentParser &p, const std::string &name_, const std::string &description_ ,
                  std::initializer_list<FlagId> flags_, const T &default_value):
      Parameter(p, name_, description_, flags_), value(default_value) {}
    /* virtual */void update(std::vector<std::string> &args) override
    {
      for (auto &f: this->flags)
        {
          std::string out("-");
          if (f.isShort)
            out.append(1, f.shortId);
          else
            {
              out.append("-"+f.longId);
            }
          auto it = std::find(args.begin(), args.end(), out );
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
    /* virtual */void show(std::ostream &os) override
    {
      for (auto &f: this->flags)
        {
          std::string out("-");
          if (f.isShort)
            out.append(1, f.shortId);
          else
            {
              out.append("-"+f.longId);
            }
          os << out << ", ";
        }
      os << "\t" << this->description ;
    }

    T get()
    {
      return this->value;
    }

  };
}

std::ostream &operator<<(std::ostream &os, const args::ArgumentParser &parser);
