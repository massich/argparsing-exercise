#include <gtest/gtest.h>

#include "argparse.h"

TEST (dummy, dummy)
{
  ASSERT_EQ(0, 0);
}

TEST ( argparse_exception_throw, help_exception_if_help_flag_or_noargs)
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::HelpFlag help ( parser, "help", "Display this help menu", {'h', "help"} );
  ASSERT_NO_THROW ( parser.ParseArgs(std::vector<std::string>{}) );
  ASSERT_THROW ( args::Help, parser.ParseArgs(std::vector<std::string>{"--help"}) );
}

TEST ( argparse_exception_throw, unknon_flag_exception )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::HelpFlag help ( parser, "help", "Display this help menu", {'h', "help"} );
  ASSERT_NO_THROW ( parser.ParseArgs(std::vector<std::string>{}) );
  ASSERT_THROW ( args::ParseError, parser.ParseArgs(std::vector<std::string>{"--Help"}) );
  ASSERT_THROW ( args::ParseError, parser.ParseArgs(std::vector<std::string>{"-H"}) );
}

TEST ( argparse, boolean_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"} );
  args::Flag bar ( parser, "BAR", "test flag", {'b', "bar"} );
  args::Flag baz ( parser, "BAZ", "test flag", {'a', "baz"} );
  parser.ParseArgs ( std::vector<std::string>{"--foo", "-b"} );
  ASSERT_EQ ( true,  foo );
  ASSERT_EQ ( true,  bar );
  ASSERT_EQ ( false, baz );
}

TEST ( argparse, boolean_flags_cluster )
{
  args::ArgumentParser parser("This is a test program.", "This goes after the options.");
  args::Flag foo(parser, "FOO", "test flag", {'f', "foo"});
  args::Flag bar(parser, "BAR", "test flag", {'b', "bar"});
  parser.ParseArgs(std::vector<std::string>{"-fb"});
  ASSERT_EQ ( true,  foo );
  ASSERT_EQ ( true,  bar );
}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
