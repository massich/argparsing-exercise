#include <iostream>
#include <vector>
#include "src/argparse.h"

int main(int argc, char **argv)
{
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"} );
  parser.parseArgs(argc, argv);
  std::cout << parser;
  std::cout << "foo is " << foo.get();

  return 0;
}
