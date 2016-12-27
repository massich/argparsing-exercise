#include <gtest/gtest.h>
#include <vector>

#include "argparse.h"

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
  args::ParameterXX<int> foo ( parser, "FOO", "test flag", {'f', "foo"}, 0 );
  args::ParameterXX<int> bar ( parser, "BAR", "test flag", {'b', "bar"}, 0 );
  args::ParameterXX<int> baz ( parser, "BAZ", "test flag", {'a', "baz"}, 0 );
  parser.parseArgs ( std::vector<std::string>{"--foo", "700", "-b", "8"} );
  ASSERT_EQ ( 700,  foo.get());
  ASSERT_EQ ( 8,  bar.get() );
  ASSERT_EQ ( 0, baz.get() );
}

TEST ( argparse, char_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::ParameterXX<char> foo ( parser, "FOO", "test flag", {'f', "foo"}, 'a' );
  args::ParameterXX<char> bar ( parser, "BAR", "test flag", {'b', "bar"}, 'a' );
  args::ParameterXX<char> baz ( parser, "BAZ", "test flag", {'a', "baz"}, 'a' );
  parser.parseArgs ( std::vector<std::string>{"--foo", "f", "-b", "b"} );
  ASSERT_EQ ( 'f',  foo.get());
  ASSERT_EQ ( 'b',  bar.get() );
  ASSERT_EQ ( 'a', baz.get() );
}

TEST ( argparse, string_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::ParameterXX<std::string> foo ( parser, "FOO", "test flag", {'f', "foo"}, "defalut" );
  args::ParameterXX<std::string> bar ( parser, "BAR", "test flag", {'b', "bar"}, "defalut" );
  args::ParameterXX<std::string> baz ( parser, "BAZ", "test flag", {'a', "baz"}, "default" );
  parser.parseArgs ( std::vector<std::string>{"--foo", "foo_string", "-b", "bar_string"} );
  ASSERT_EQ ( "foo_string",  foo.get());
  ASSERT_EQ ( "bar_string",  bar.get() );
  ASSERT_EQ ( "default",  baz.get() );
}

TEST ( argparse, DISABLED_string_with_spaces )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::ParameterXX<std::string> foo ( parser, "FOO", "test flag", {'f', "foo"}, "defalut" );
  parser.parseArgs ( std::vector<std::string>{"--foo", "\"foo string\"", "-b", "bar_string"} );
  std::cout << foo.get() << std::endl;
  ASSERT_EQ ( "\"foo string\"",  foo.get() );
}

 TEST ( argparse, multi_parameter )
 {
   args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
   args::MultiParameter<int, 1> foo ( parser, "FOO", "test single mult-parameter", {'f', "foo"});
   args::MultiParameter<int, 2> bar ( parser, "BAR", "test two elements multi-parameter", {'b', "bar"});
   parser.parseArgs ( std::vector<std::string>{"--foo", "700", "-b", "8", "10"} );

   std::array<int,1> foo_answer = {700};
   std::array<int,2> bar_answer = {8, 10};

   ASSERT_EQ ( foo_answer,  foo.get());
   ASSERT_EQ ( bar_answer,  bar.get() );
 }


int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
