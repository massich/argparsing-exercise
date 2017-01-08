#include <gtest/gtest.h>
#include <vector>

#include "argparse.h"

namespace{
  const int get_argc(const char* const *argv)
  {
    int count = 0;
    const char * const * it = argv;
    for (; (*it!=NULL) ; ++count, ++it) {}
    return count;
  }

}


TEST ( argparse_exception_throw, type_mismatch )
{
  const char* argv[] = {"./test", "-f", "this is not an integer", NULL};
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<int> foo ( parser, "FOO", "test integer", {'f', "foo"}, 0 );
  ASSERT_THROW ( parser.parseArgs( get_argc(argv), argv), args::ParseError );
}

TEST ( argparse_exception_throw, unknown_parameter )
{
  const char* argv[] = {"./test", "--foo", "--bar", NULL};
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"} );
  ASSERT_THROW ( parser.parseArgs( get_argc(argv), argv), args::UnknownParameter );
}


TEST ( argparse, boolean_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"} );
  args::Flag bar ( parser, "BAR", "test flag", {'b', "bar"} );
  args::Flag baz ( parser, "BAZ", "test flag", {'a', "baz"} );
  parser.parseArgs ( std::vector<std::string>{"./test", "--foo", "-b"} );
  ASSERT_EQ ( true,  foo.get());
  ASSERT_EQ ( true,  bar.get() );
  ASSERT_EQ ( false, baz.get() );
}

TEST ( argparse, int_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<int> foo ( parser, "FOO", "test flag", {'f', "foo"}, 0 );
  args::Parameter<int> bar ( parser, "BAR", "test flag", {'b', "bar"}, 0 );
  args::Parameter<int> baz ( parser, "BAZ", "test flag", {'a', "baz"}, 0 );
  parser.parseArgs ( std::vector<std::string>{"./test", "--foo", "700", "-b", "8"} );
  ASSERT_EQ ( 700,  foo.get());
  ASSERT_EQ ( 8,  bar.get() );
  ASSERT_EQ ( 0, baz.get() );
}

TEST ( argparse, char_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<char> foo ( parser, "FOO", "test flag", {'f', "foo"}, 'a' );
  args::Parameter<char> bar ( parser, "BAR", "test flag", {'b', "bar"}, 'a' );
  args::Parameter<char> baz ( parser, "BAZ", "test flag", {'a', "baz"}, 'a' );
  parser.parseArgs ( std::vector<std::string>{"./test", "--foo", "f", "-b", "b"} );
  ASSERT_EQ ( 'f',  foo.get());
  ASSERT_EQ ( 'b',  bar.get() );
  ASSERT_EQ ( 'a', baz.get() );
}

TEST ( argparse, string_flags )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<std::string> foo ( parser, "FOO", "test flag", {'f', "foo"}, "defalut" );
  args::Parameter<std::string> bar ( parser, "BAR", "test flag", {'b', "bar"}, "defalut" );
  args::Parameter<std::string> baz ( parser, "BAZ", "test flag", {'a', "baz"}, "default" );
  parser.parseArgs ( std::vector<std::string>{"./test", "--foo", "foo_string", "-b", "bar_string"} );
  ASSERT_EQ ( "foo_string",  foo.get());
  ASSERT_EQ ( "bar_string",  bar.get() );
  ASSERT_EQ ( "default",  baz.get() );
}

TEST ( argparse, string_with_spaces )
{
  const char* argv[] = {"./test", "--foo", "\"foo string\"", NULL};
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<std::string> foo ( parser, "FOO", "test flag", {'f', "foo"}, "defalut" );
  parser.parseArgs ( get_argc(argv), argv );
  std::cout << foo.get() << std::endl;
  ASSERT_EQ ( "\"foo string\"",  foo.get() );
}

// using MyType = std::tuple<int,std::string>;
struct MyType{
  int i;
  std::string s;
};

bool operator==(const MyType &a, const MyType &b)
{
  return std::tie(a.i, a.s) == std::tie(b.i, b.s);
}

std::ostream& operator << (std::ostream& o, const MyType &element)
{
  return o << element.i << ", " << element.s ;
}

std::istream& operator>>(std::istream &input,MyType &o)
{
  return input >> o.i >> o.s;
}


TEST ( argparse, multi_parameter )
{
  const char* argv[] = {"./test", "--foo", "0", "xxxx", NULL};

  const MyType foo_default{42,"foo"};
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<MyType> foo ( parser, "FOO", "test single mult-parameter", {'f', "foo"}, foo_default );
  parser.parseArgs ( get_argc(argv), argv );

  const MyType foo_expected{0,"xxxx"};
  const MyType foo_answer = foo.get();

  ASSERT_EQ ( foo_expected,  foo_answer);
}

TEST ( argparse, multi_parameter_default )
{
  const char* argv[] = {"./test", NULL};

  const MyType foo_default{42,"foo"};
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Parameter<MyType> foo ( parser, "FOO", "test single mult-parameter", {'f', "foo"}, foo_default );
  parser.parseArgs ( get_argc(argv), argv );

  const MyType foo_answer = foo.get();

  ASSERT_EQ ( foo_default,  foo_answer);
}


TEST ( argparse_string2tokens, when_empty)
{
  const std::string input="./test";
  const char* argv = &input[0];
  const auto answer = args::string2tokens(1, &argv );
  const std::vector<std::string> expected_answer {"./test"};
  ASSERT_EQ( expected_answer, answer );
}

TEST ( argparse_string2tokens, when_flag)
{
  const char* argv[] = {"./test", "--foo", NULL};
  const int argc = sizeof(argv) / sizeof(char*) -1;
  const auto answer = args::string2tokens(argc, argv );
  const std::vector<std::string> expected_answer {"./test", "--foo", ""};
  ASSERT_EQ( expected_answer, answer );
}

TEST ( argparse_string2tokens, when_element)
{
  const char* argv[] = {"./test", "--foo", "FOO", "3", NULL};
  const auto answer = args::string2tokens( get_argc(argv), argv );
  const std::vector<std::string> expected_answer {"./test", "--foo", " FOO 3"};
  ASSERT_EQ( expected_answer, answer );

}

TEST ( argparse_actions, simple_call )
{
  int value = 0;
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Flag foo ( parser, "FOO", "test flag", {'f', "foo"}, [&value](){ value++;} );
  parser.parseArgs ( std::vector<std::string>{"./test", "--foo" });
  parser.process();
  ASSERT_EQ ( 1,  value );
}

TEST ( argparse_actions, display_help )
{
  args::ArgumentParser parser ( "This is a test program.", "This goes after the options." );
  args::Flag help ( parser, "HELP", "help flag", {'h', "help"}, [&parser](){ std::cout << parser << std::endl;} );
  parser.parseArgs ( std::vector<std::string>{"./test", "-h" });
  parser.process();
}

TEST ( argparse_helper_functions, flagId_to_string)
{
  const args::FlagId f = 'f';
  const args::FlagId foo = "foo";
  ASSERT_EQ ( "-f", flagId_to_string(f) );
  ASSERT_EQ ( "--foo", flagId_to_string(foo) );
}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
