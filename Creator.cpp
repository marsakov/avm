#include "Creator.hpp"
#include "Operand.hpp"

Creator::Creator() { }

Creator::~Creator() { }

IOperand const * Creator::createInt8( std::string const & value ) const {
    int op;

    try {
        op = std::stoi(value);
    }
    catch (std::exception & e)
    {
        throw Exception("Overflow on value");
    }
    if (op > 128 || op < -127)
        throw Exception("Overflow on value");
	return (new Operand<int8_t>(value, Int8));
}

IOperand const * Creator::createInt16( std::string const & value ) const {
    int op;

    try {
        op = std::stoi(value);
    }
    catch (std::exception & e)
    {
        throw Exception("Overflow on value");
    }
    if (op > 32767 || op < -32768)
        throw Exception("Overflow on value");
	return (new Operand<int16_t>(value, Int16));
}

IOperand const * Creator::createInt32( std::string const & value ) const {
    int op;

    try {
        op = std::stoi(value);
    }
    catch (std::exception & e)
    {
        throw Exception("Overflow on value");
    }
    op = std::stof(value);
    if (op > 2147483648 || op < -2147483647)
        throw Exception("Overflow on value");
	return (new Operand<int32_t>(value, Int32));
}

IOperand const * Creator::createFloat( std::string const & value ) const {
    try {
        std::stof(value);
    }
    catch (std::exception & e)
    {
        throw Exception("Overflow on value");
    }

	return (new Operand<float>(value, Float));
}

IOperand const * Creator::createDouble( std::string const & value ) const {
    try {
        std::stod(value);
    }
    catch (std::exception & e)
    {
        throw Exception("Overflow on value");
    }
	return (new Operand<double>(value, Double));
}

IOperand const * Creator::createOperand( std::string const & value, eOperandType type ) const {
	if (type == Int8) {
		return (createInt8(value));
	} else if (type == Int16) {
		return (createInt16(value));
	} else if (type == Int32) {
		return (createInt32(value));
	} else if (type == Float) {
		return (createFloat(value));
	} else {
		return (createDouble(value));
	}
}

IOperand const * Creator::createOperand( IOperand const *rhs ) const {
    if (rhs->getType() == Int8) {
        return (createInt8(rhs->toString()));
    } else if (rhs->getType() == Int16) {
        return (createInt16(rhs->toString()));
    } else if (rhs->getType() == Int32) {
        return (createInt32(rhs->toString()));
    } else if (rhs->getType() == Float) {
        return (createFloat(rhs->toString()));
    } else {
        return (createDouble(rhs->toString()));
    }
}