# Argument Parser Exercise

This is a coding exercise to build an small and portable library for parsing command line arguments.

# Usage

The idea is to create an `ArgumentParser` object, then create as many parameters
as needed. The parameters can be of type `Parameter<T>`, `Flag`, or any derivate as long as `cin` is provided.
`Parameter<T>` takes arguments while `Flag` take no arguments.
Both `Parameter<T>` and `Flag` can be instantiated with a callback function.

Use `ArgumentParser::parseArgs(...)` to parse the given call and set their values.
`ArgumentParser::process()` to trigger the callback of all the parameters found in the call. (This process requires `parseArgs` to be runned before). `ArgumentParser::processArgs(...)` is a convenient function that run both `parseArgs` and `process`.

# Examples

All the code examples here will be complete code examples, with some output.

## Simple example:

```cpp
#include <iostream>
#include <stdlib.h> /* exit, EXIT_FAILURE */
#include "../src/argparse.h"

void myHelp_callback( args::ArgumentParser &p )
{
  std::cout << p;
  exit (0);
}

int main(int argc, char **argv)
{
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  args::Flag help ( parser, "HELP", "help flag", {'h', "help"},
                    [&parser](){ myHelp_callback(parser); } );

  try {
    parser.processArgs(argc, argv);
  } catch (args::Error e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    exit (EXIT_FAILURE);
  }

  std::cout << "foo bar bas" << std::endl;
  return 0;
}
```

```shell
 % ./test
 foo bar bas
 % ./test -h
  This is a test program.

-h, --help,     help flag
  This goes after the options.

 % ./test --wrong_parameter xx
2 unknown parameters: --wrong_parameter, xx,

  This is a test program.

-h, --help,     help flag
  This goes after the options.
 %
```

## Different construction examples of basic type parameters.

```cpp
#include <iostream>
#include <stdlib.h> /* exit, EXIT_FAILURE */
#include "argparse.h"

int main(int argc, char **argv)
{
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  args::Parameter<int> foo ( parser, "FOO", "test flag takes integer", {'f', "foo"}, 0 );
  args::Parameter<std::string> bar ( parser, "BAR", "test flag takes string", {'b', "bar"}, "default string" );
  args::Parameter<bool> bas ( parser, "BAS", "test binary flag with argument", {"bas"}, true );
  args::Flag baz ( parser, "BAZ", "test flag with no arguments", {'z'} );

  try {
    parser.processArgs(argc, argv);
  } catch (args::Error e)
    {
      std::cerr << e.what() << std::endl;
      std::cerr << parser;
      exit (EXIT_FAILURE);
    }

  std::cout << "foo: " << foo.get() << std::endl;
  std::cout << "bar: " << bar.get() << std::endl;
  std::cout << "bas: " << bas.get() << std::endl;
  std::cout << "baz: " << baz.get() << std::endl;
  return 0;
}
```

```shell
 % ./test 
foo: 0
bar: default string
bas: 1
baz: 0

 % ./test --unknown option
2 unknown parameters: --unknown, option,

  This is a test program.

-f, --foo,      test flag takes integer
-b, --bar,      test flag takes string
--bas,  test binary flag with argument
-z,     test flag with no arguments
  This goes after the options.

 % ./test -b "this is the called string" --foo 10 --bas 0 -z
foo: 10
bar: this is the called string
bas: 0
baz: 1

 % ./test --bar this is another valid string
foo: 0
bar: this is another valid string
bas: 1
baz: 0

 % ./test -f a
"a" can't be converted into i

  This is a test program.

-f, --foo,      test flag takes integer
-b, --bar,      test flag takes string
--bas,  test binary flag with argument
-z,     test flag with no arguments
  This goes after the options.
```

## Cutsom types

```cpp
#include <iostream>
#include <stdlib.h> /* exit, EXIT_FAILURE */
#include "argparse.h"

struct Person{
  std::string name;
  int age;
  std::string tel;
};

std::istream& operator>>(std::istream &input, Person &p)
{
  return input >> p.name >> p.age >> p.tel;
}

int main(int argc, char **argv)
{
  Person default_person = {"mr. X", 42, "+0 (0) 000 000 000"};
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  args::Parameter<Person> alice ( parser, "ALICE", "test parameter", {'a', "alice", "Alice", "ALICE"}, default_person );

  try {
    parser.processArgs(argc, argv);
  } catch (args::Error e)
    {
      std::cerr << e.what() << std::endl;
      std::cerr << parser;
      exit (EXIT_FAILURE);
    }

  std::cout << alice.get().name << std::endl;
  std::cout << alice.get().age << std::endl;
  std::cout << alice.get().tel << std::endl;

  return 0;
}

```

```shell
% ./test -a alice 24 +1-(1)-111-111-111
alice
24
+1-(1)-111-111-111
```
