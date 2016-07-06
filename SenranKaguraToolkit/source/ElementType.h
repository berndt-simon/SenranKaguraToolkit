#pragma once

#include <string>

static const std::string TYPE_KEY_GXT = "GXT";
static const std::string TYPE_KEY_TMD = "TMD";
static const std::string TYPE_KEY_TMD_TOON = "TMD_TOON";
static const std::string TYPE_KEY_UNDEFINED = "UNDEFINDED";

enum class ElementType_e {
	GXT, TMD, TMD_TOON, UNDEFINED
};

inline std::string to_string(ElementType_e type) {
	switch (type) {
		case ElementType_e::GXT:
			return TYPE_KEY_GXT;
		case ElementType_e::TMD:
			return TYPE_KEY_TMD;
		case ElementType_e::TMD_TOON:
			return TYPE_KEY_TMD_TOON;
	}
	return TYPE_KEY_UNDEFINED;
}

inline ElementType_e from_string(const std::string& type_name) {
	if (TYPE_KEY_GXT.compare(type_name) == 0) {
		return ElementType_e::GXT;
	} else if (TYPE_KEY_TMD.compare(type_name) == 0) {
		return ElementType_e::TMD;
	} else if (TYPE_KEY_TMD_TOON.compare(type_name) == 0) {
		return ElementType_e::TMD_TOON;
	}
	return ElementType_e::UNDEFINED;
}