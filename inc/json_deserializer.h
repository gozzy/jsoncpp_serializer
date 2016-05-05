#pragma once

// Simple header-only class deserializer based on jsoncpp library.

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include <json/json.h>

namespace Serialization {

using namespace std;

class Deserializer
{
public:
	explicit Deserializer(Json::Value &r)
		: root(r)
	{
		// no-op
	}

	template <typename T>
	bool operator() (const string &key, T &res) const
	{
		try {
			deserialize(root[key], res);
		} catch (const exception &e) {
			cerr << "Failed to deserialize key \"" << key << "\": " << e.what() << endl;
			return false;
		}
		
		return true;
	}

protected:
	void deserialize(Json::Value val, uint32_t &res) const
	{
		res = val.asUInt();
	}

	void deserialize(Json::Value val, int32_t &res) const
	{
		res = val.asInt();
	}

	void deserialize(Json::Value val, uint64_t &res) const
	{
#ifdef JSON_HAS_INT64
		res = val.asUInt64();
#else
		res = val.asUInt();
#endif
	}

	void deserialize(Json::Value val, int64_t &res) const
	{
#ifdef JSON_HAS_INT64
		res = val.asInt64();
#else
		res = val.asInt();
#endif
	}

	void deserialize(Json::Value val, float &res) const
	{
		res = val.asFloat();
	}

	void deserialize(Json::Value val, double &res) const
	{
		res = val.asDouble();
	}

	void deserialize(Json::Value val, string &res) const
	{
		res = val.asString();
	}

	template<typename T>
	void deserialize(Json::Value val, vector<T> &res) const
	{
		for (const Json::Value &arr_item : val) {
			T vec_item;
			deserialize(arr_item, vec_item);
			res.push_back(vec_item);
		}
	}

	template<typename KeyType, typename ValueType>
	void deserialize(Json::Value val, map<KeyType, ValueType> &res) const
	{
		KeyType k;
		ValueType v;

		for (const auto &item : val.getMemberNames()) {
			deserialize(item, k);
			deserialize(val[item], v);
			res[k] = v;
		}
	}

	template<typename KeyType, typename ValueType>
	void deserialize(Json::Value val, pair<KeyType, ValueType> &res) const
	{
		KeyType k;
		ValueType v;

		deserialize(val["1st"], k);
		deserialize(val["2nd"], v);
		res = { k, v };
	}

	template<typename T>
	void deserialize(Json::Value val, T &res) const
	{
		Deserializer d(val);
		res.deserialize(d);
	}

	Json::Value &root;
};

template <class T>
bool fromJSON(const string &txt, T &obj)
{
	Json::Value root;
	Json::Reader reader;
	bool ret = false;

	if (reader.parse(txt, root)) {
		Deserializer d(root);
		ret = obj.deserialize(d);
	}

	return ret;
}

}	// namespace Serialization
