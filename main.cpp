#include <iostream>
#include "src/argparse.h"

int main(int argc, char **argv)
{
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  std::cout << parser;
  parser.parseArgs("--hi");
  return 0;
}
