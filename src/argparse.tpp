template < typename T >
void args::ParameterXX<T>::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}

template < typename T >
void args::ParameterXX<T>::update(std::vector<std::string> &args)
{
  for (auto &f: this->flags)
    {
      auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
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

template < typename T >
T args::ParameterXX<T>::get()
{
  return this->value;
}


template < typename T, std::size_t N >
void args::MultiParameter<T,N>::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}

template <typename T, std::size_t N>
void args::MultiParameter<T, N>::update(std::vector<std::string> &args) {
  for (auto &f : this->flags) {
    auto it = std::find(args.begin(), args.end(), flagId_to_string(f));
    if (it != args.end()) {
      for (auto i = 0; i < N; ++i) {
        auto element_it = it;
        std::stringstream(*(++element_it)) >> this->value[i];
        element_it = args.erase(element_it);
      }
      it = args.erase(it);
      break;
    }
  }
}

template < typename T, std::size_t N >
std::array<T,N> args::MultiParameter<T,N>::get()
{
  for ( auto i=0; i<N; ++i)
      {
          std::cout << '(' << i << ',' << this->value[i] <<')' << '\n';
      }
  return this->value;
}
