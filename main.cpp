#include <iostream>
#include <vector>
#include "src/argparse.h"

int main(int argc, char **argv)
{
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"} );
  std::cout << parser;
  parser.parseArgs(argc, argv);

  return 0;
}
