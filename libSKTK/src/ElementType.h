#pragma once

#include <string>

enum class ElementType_e {
	GXT, TMD, TMD_TOON, UNDEFINED
};

std::string to_string(ElementType_e type);

ElementType_e from_string(const std::string& type_name);