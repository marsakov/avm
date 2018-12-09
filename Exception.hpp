

#ifndef AVM_EXCEPTION_HPP
#define AVM_EXCEPTION_HPP


class Exception {

    char const * _msg;

public:
    Exception(char const * msg);
    Exception(void);
    Exception(Exception const & rhs);

    Exception	&operator=(Exception const &rhs);
    char const *what() const throw();

    ~Exception() throw();

};


#endif