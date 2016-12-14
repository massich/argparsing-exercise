#include <gtest/gtest.h>
#include <vector>

#include "argparse.h"

TEST (dummy, dummy)
{
  ASSERT_EQ(0, 0);
}

TEST ( argparse_exception_throw, help_exception_if_help_flag_or_noargs)
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::HelpFlag help ( parser, "help", "Display this help menu", {'h', "help"} );
  ASSERT_NO_THROW ( parser.parseArgs(std::vector<std::string>{}) );
  ASSERT_THROW ( parser.parseArgs("--help"), args::Help );
}

// TEST ( argparse_exception_throw, unknon_flag_exception )
// {
//   args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
//   args::HelpFlag help ( parser, "help", "Display this help menu", {'h', "help"} );
//   ASSERT_NO_THROW ( parser.ParseArgs(std::vector<std::string>{}) );
//   ASSERT_THROW ( args::ParseError, parser.ParseArgs(std::vector<std::string>{"--Help"}) );
//   ASSERT_THROW ( args::ParseError, parser.ParseArgs(std::vector<std::string>{"-H"}) );
// }

TEST ( argparse, boolean_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"} );
  args::Flag bar ( parser, "BAR", "test flag", {'b', "bar"} );
  args::Flag baz ( parser, "BAZ", "test flag", {'a', "baz"} );
  parser.parseArgs ( std::vector<std::string>{"--foo", "-b"} );
  ASSERT_EQ ( true,  foo.get());
  ASSERT_EQ ( true,  bar.get() );
  ASSERT_EQ ( false, baz.get() );
}

TEST ( argparse, int_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::ParameterInt foo ( parser, "FOO", "test flag", {'f', "foo"}, 0 );
  args::ParameterInt bar ( parser, "BAR", "test flag", {'b', "bar"}, 0 );
  args::ParameterInt baz ( parser, "BAZ", "test flag", {'a', "baz"}, 0 );
  parser.parseArgs ( std::vector<std::string>{"--foo", "700", "-b", "8"} );
  ASSERT_EQ ( 700,  foo.get());
  ASSERT_EQ ( 8,  bar.get() );
  ASSERT_EQ ( 0, baz.get() );
}

TEST ( argparse, char_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::ParameterChar foo ( parser, "FOO", "test flag", {'f', "foo"}, 'a' );
  args::ParameterChar bar ( parser, "BAR", "test flag", {'b', "bar"}, 'a' );
  args::ParameterChar baz ( parser, "BAZ", "test flag", {'a', "baz"}, 'a' );
  parser.parseArgs ( std::vector<std::string>{"--foo", "f", "-b", "b"} );
  ASSERT_EQ ( 'f',  foo.get());
  ASSERT_EQ ( 'b',  bar.get() );
  ASSERT_EQ ( 'a', baz.get() );
}

TEST ( argparse, string_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::ParameterStr foo ( parser, "FOO", "test flag", {'f', "foo"}, "defalut" );
  args::ParameterStr bar ( parser, "BAR", "test flag", {'b', "bar"}, "defalut" );
  args::ParameterStr baz ( parser, "BAZ", "test flag", {"defalut", "baz"}, "default" );
  parser.parseArgs ( std::vector<std::string>{"--foo", "foo_string", "-b", "bar_string"} );
  ASSERT_EQ ( "foo_string",  foo.get());
  ASSERT_EQ ( "bar_string",  bar.get() );
  ASSERT_EQ ( "default",  baz.get() );
}

// TEST ( argparse, boolean_flags_cluster )
// {
//   args::ArgumentParser parser("This is a test program.", "This goes after the options.");
//   args::Flag foo(parser, "FOO", "test flag", {'f', "foo"});
//   args::Flag bar(parser, "BAR", "test flag", {'b', "bar"});
//   parser.ParseArgs(std::vector<std::string>{"-fb"});
//   ASSERT_EQ ( true,  foo );
//   ASSERT_EQ ( true,  bar );
// }

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
