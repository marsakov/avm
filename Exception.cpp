
#include "Exception.hpp"


Exception::Exception(char const * msg) : _msg(msg) {}

Exception::Exception(void) : _msg("Error") {}

Exception::Exception(Exception const & rhs) {
    *this = rhs;
}

Exception	&Exception::operator=(Exception const &rhs){
    this->_msg = rhs._msg;
    return (*this);
}

char const *Exception::what() const throw() {
    return (_msg);
}

Exception::~Exception() throw() {}