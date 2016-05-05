#pragma once

// Simple header-only class serializer based on jsoncpp library.

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include <json/json.h>

namespace Serialization {

using namespace std;

class Serializer
{
public:
	string toString() const
	{
		return root.toStyledString();
	}

	template<typename T>
	void operator () (const string &key, const T &value)
	{
		serialize(key, value);
	}

protected:
	Json::Value root;

	template<typename T>
	void storeValue(const string &key, const T &val)
	{
		if (root[key].type() == Json::arrayValue) {
			root[key].append(val);
		} else {
			root[key] = val;
		}
	}

	void serialize(const string &key, const string &val)
	{
		storeValue(key, val);
	}

	void serialize(const string &key, const uint32_t &val)
	{
		storeValue(key, val);
	}

	void serialize(const string &key, const int32_t &val)
	{
		storeValue(key, val);
	}

	void serialize(const string &key, const uint64_t val)
	{
#ifdef JSON_HAS_INT64
		storeValue(key, (Json::Value::UInt64)val);
#else
		storeValue(key, (Json::Value::UInt)val);
#endif
	}

	void serialize(const string &key, const int64_t val)
	{
#ifdef JSON_HAS_INT64
		storeValue(key, (Json::Value::Int64)val);
#else
		storeValue(key, (Json::Value::Int)val);
#endif
	}

	void serialize(const string &key, const float &val)
	{
		storeValue(key, val);
	}

	void serialize(const string &key, const double &val)
	{
		storeValue(key, val);
	}

	void serialize(const string &key, const bool &val)
	{
		storeValue(key, val);
	}

	template<typename T>
	void serialize(const string &key, const T &val)
	{
		Serializer s;
		val.serialize(s);

		if (root[key].type() == Json::arrayValue) {
			root[key].append(s.root);
		} else {
			root[key] = s.root;
		}
	}

	template<typename T>
	void serialize(const string &key, const vector<T> &val)
	{
		root[key] = Json::arrayValue;
		for (const auto &i : val) {
			serialize(key, i);
		}
	}

	template<typename T>
	void serialize(const string &key, const set<T> &val)
	{
		root[key] = Json::arrayValue;
		for (const auto &i : val) {
			serialize(key, i);
		}
	}

	template<typename KeyType, typename ValueType>
	void serialize(const string &key, const map<KeyType, ValueType> &val)
	{
		Serializer s;
		root[key] = Json::objectValue;

		for (const auto &kv : val) {
			s.serialize(kv.first, kv.second);
		}

		root[key] = s.root;
	}

	template<typename KeyType, typename ValueType>
	void serialize(const string &key, const pair<KeyType, ValueType> &val)
	{
		Serializer s;
		root[key] = Json::objectValue;

		s.serialize("1st", val.first);
		s.serialize("2nd", val.second);

		root[key] = s.root;
	}
};

template <class T>
string toJSON(const T &obj)
{
	Serializer s;
	obj.serialize(s);
	return s.toString();
}

}	// namespace Serialization
