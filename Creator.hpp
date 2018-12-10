#pragma once

#include "IOperand.h"
#include "Exception.hpp"

class Creator {

	std::string	 _value;
	eOperandType _type;

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;

public:

	Creator();
	~Creator();
	IOperand const * createOperand ( std::string const & value, eOperandType type ) const;
	IOperand const * createOperand( IOperand const *rhs ) const;
};
