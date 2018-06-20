#include "Cvar.h"
#include <memory>

Cvar::Cvar()
	: m_name()
	, m_string()
	, m_resetString()
	, m_latchedString()
	, m_flags(0)
	, m_modified(false)
	, m_modificationCount(0)
	, m_value(0)
	, m_integer(0)
{}

const char *Cvar::name() const {
	return m_name.c_str();
}

const char *Cvar::string() const {
	return m_string.c_str();
}

const char *Cvar::resetString() const {
	return m_resetString.c_str();
}

const char *Cvar::latchedString() const {
	return m_latchedString.c_str();
}

int Cvar::flags() const {
	return m_flags;
}

bool Cvar::modified() const {
	return m_modified;
}

int Cvar::modificationCount() const {
	return m_modificationCount;
}

float Cvar::value() const {
	return m_value;
}

int Cvar::integer() const {
	return m_integer;
}

bool Cvar::has_flag(int flag) const {
	return m_flags & flag;
}

void Cvar::remove_flag(int flag) {
	m_flags &= ~flag;
}

void Cvar::add_flag(int flag) {
	m_flags |= flag;
}

Cvar::operator Cvar*() & {
	return this;
}