#include <iostream>
#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include "json_serializer.h"
#include "json_deserializer.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(SerializingTests)

BOOST_AUTO_TEST_CASE(SimpleTest)
{
	struct A {
		uint32_t num;

		void serialize(Serialization::Serializer &s) const
		{
			s("num", num);
		}

		bool deserialize(const Serialization::Deserializer &d)
		{
			return d("num", num);
		}
	};

	A a1 = { 125 }, a2 = {};
	string json = Serialization::toJSON(a1);

	BOOST_REQUIRE(Serialization::fromJSON(json, a2));
	BOOST_REQUIRE_EQUAL(a1.num, a2.num);
}

BOOST_AUTO_TEST_CASE(STLTest)
{
	struct A {
		string str;
		vector<float> vec;

		void serialize(Serialization::Serializer &s) const
		{
			s("str", str);
			s("vec", vec);
		}

		bool deserialize(const Serialization::Deserializer &d)
		{
			return d("str", str) && d("vec", vec);
		}
	};

	A a1, a2 = {};
	a1.str = "Hello world";
	a1.vec = { 1.23, 4.56, 7.89 };
	string json = Serialization::toJSON(a1);

	BOOST_REQUIRE(Serialization::fromJSON(json, a2));
	BOOST_REQUIRE_EQUAL(a1.str, a2.str);
	BOOST_CHECK_EQUAL_COLLECTIONS(a1.vec.begin(), a1.vec.end(), a2.vec.begin(), a2.vec.end());
}

BOOST_AUTO_TEST_CASE(ComplexNestedTest)
{
	struct A
	{
		int num;
		string str;

		void serialize(Serialization::Serializer &s) const
		{
			s("num", num);
			s("str", str);
		}

		bool deserialize(const Serialization::Deserializer &d)
		{
			return d("num", num) && d("str", str);
		}
	};

	struct B
	{
		A a;
		vector<string> vec;
		map<string, string> m;
		pair<string, string> p;
		double dbl;

		void serialize(Serialization::Serializer &s) const
		{
			s("a", a);
			s("vec", vec);
			s("map", m);
			s("pair", p);
			s("dbl", dbl);
		}

		bool deserialize(const Serialization::Deserializer &d)
		{
			bool suc = d("a", a);
			suc &= d("vec", vec);
			suc &= d("map", m);
			suc &= d("pair", p);
			suc &= d("dbl", dbl);
			return suc;
		}
	};

	B b1, b2 = {};
	b1.a = { 22, "Hello world" };
	b1.vec = { "item1", "item2", "item3" };
	b1.m = {{"key1", "value1"}, {"key2", "value2"}};
	b1.p = { "pair_key", "pair_val" };
	b1.dbl = 1.23456;

	string json = Serialization::toJSON(b1);

	BOOST_REQUIRE(Serialization::fromJSON(json, b2));
	BOOST_REQUIRE_EQUAL(b1.a.num, b2.a.num);
	BOOST_REQUIRE_EQUAL(b1.a.str, b2.a.str);
	BOOST_CHECK_EQUAL_COLLECTIONS(b1.vec.begin(), b1.vec.end(), b2.vec.begin(), b2.vec.end());
	BOOST_REQUIRE(b1.m == b2.m);
	BOOST_REQUIRE(b1.p == b2.p);
	BOOST_REQUIRE_EQUAL(b1.dbl, b2.dbl);
}

BOOST_AUTO_TEST_SUITE_END()
