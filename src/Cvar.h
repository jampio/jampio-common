#pragma once

#include <string>

class CvarSystem;

class Cvar {
	friend class CvarSystem;
private:
	std::string m_name;
	std::string m_string;
	std::string m_resetString;
	std::string m_latchedString;
	int m_flags;
	bool m_modified;
	int m_modificationCount;
	float m_value;
	int m_integer;
public:
	Cvar();
	const char *name() const;
	const char *string() const;
	const char *resetString() const;
	const char *latchedString() const;
	int flags() const;
	bool modified() const;
	void setModified(bool value);
	int modificationCount() const;
	float value() const;
	int integer() const;
	bool has_flag(int flag) const;
	void remove_flag(int flag);
	void add_flag(int flag);
	operator Cvar*() &;
};