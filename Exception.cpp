
#include <iostream>
#include "Exception.hpp"


Exception::Exception(char const * msg) : _msg(msg) {
    std::cout << what() << std::endl;
}

Exception::Exception(void) : _msg("Error") {
    std::cout << what() << std::endl;
}

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