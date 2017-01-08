template < typename T >
void args::Parameter<T>::show(std::ostream &os)
{
  for (auto &f: this->flags)
    {
      os << flagId_to_string(f) + ", ";
    }
  os << "\t" << this->description ;
}

template < typename T >
void args::Parameter<T>::_update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args)
{
    if( !(std::stringstream(*(++it)) >> this->value) )
    {
      std::string flag = "\"" + *it + "\" can't be converted into " + typeid(this->value).name();
      throw ParseError(flag);
    }
    args.erase(it);
}

template < typename T >
T args::Parameter<T>::get()
{
  return this->value;
}

// template<>
// void args::Parameter<std::string>::_update_and_consume_if_necessary( std::vector<std::string>::iterator it, std::vector<std::string> &args)
// {
//     this->value = std::string(*(++it));
//     args.erase(it);
// }