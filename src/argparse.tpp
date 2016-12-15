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