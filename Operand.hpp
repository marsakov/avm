#pragma once

#include <iostream>
#include <sstream>
#include "IOperand.h"
#include "Creator.hpp"


template <typename T>
class Operand  : public IOperand {

	Creator			_creator;
	std::string		_value;
	eOperandType	_type;
	int				_precision;

public:

	Operand(std::string value, eOperandType type);
	Operand();

	virtual int 					getPrecision( void ) const;
	virtual eOperandType 			getType( void ) const;
	Operand<T>						&operator=( Operand<T> const & rhs ) const;

	virtual IOperand const * 		operator+( IOperand const & rhs ) const;
	virtual IOperand const * 		operator-( IOperand const & rhs ) const;
	virtual IOperand const * 		operator*( IOperand const & rhs ) const;
	virtual IOperand const * 		operator/( IOperand const & rhs ) const;
	virtual IOperand const * 		operator%( IOperand const & rhs ) const;
	virtual std::string const &		toString( void ) const;
	virtual ~Operand( void );

};

template <typename T>
Operand<T>::Operand(void) {}

template <typename T>
Operand<T>::Operand(std::string value, eOperandType type) {
	_value = value;
	_type = type;
}

template <typename T>
Operand<T>::~Operand(void) {}

template <typename T>
Operand<T>	&Operand<T>::operator=( Operand<T> const &rhs ) const {
	_value = rhs._value;
	_type = rhs._type;
}

template <typename T>
std::string const &		Operand<T>::toString( void ) const { return _value; }

template <typename T>
int 			Operand<T>::getPrecision( void ) const { return _precision; }

template <typename T>
eOperandType 	Operand<T>::getType( void ) const { return _type; }

template <typename T>
IOperand const	*Operand<T>::operator+( IOperand const &rhs ) const {
	std::ostringstream result;

	result << std::stod(_value) + std::stod(rhs.toString());
	return (_creator.createOperand(result.str(), (_type > rhs.getType()) ? _type : rhs.getType() ));
}

template <typename T>
IOperand const	*Operand<T>::operator-( IOperand const &rhs ) const {
	std::ostringstream result;

	result << std::stod(_value) - std::stod(rhs.toString());
	return (_creator.createOperand(result.str(), (_type > rhs.getType()) ? _type : rhs.getType() ));
}

template <typename T>
IOperand const	*Operand<T>::operator*( IOperand const &rhs ) const {
	std::ostringstream result;

	result << std::stod(_value) * std::stod(rhs.toString());
	return (_creator.createOperand(result.str(), (_type > rhs.getType()) ? _type : rhs.getType() ));
}

template <typename T>
IOperand const	*Operand<T>::operator/( IOperand const &rhs ) const {
	std::ostringstream result;

	result << std::stod(_value) / std::stod(rhs.toString());
	return (_creator.createOperand(result.str(), (_type > rhs.getType()) ? _type : rhs.getType() ));
}

template <typename T>
IOperand const	*Operand<T>::operator%( IOperand const &rhs ) const {
	std::ostringstream result;

	result << std::stoi(_value) % std::stoi(rhs.toString());
	return (_creator.createOperand(result.str(), (_type > rhs.getType()) ? _type : rhs.getType() ));
}
