#include <string>
using namespace std;
class VALUE
{
public:
	VALUE();
	~VALUE();

private:

};

VALUE::VALUE()
{
}

VALUE::~VALUE()
{
}
class OBJECT
{
public:
	OBJECT();
	~OBJECT();

	OBJECT(const OBJECT& temp);
	OBJECT(const std::string &ss, const VALUE &vv);
	template<size_t N>
	OBJECT(const char(&key)[N], const VALUE& value);

};

OBJECT::OBJECT()
{
}

OBJECT::~OBJECT()
{
}
class ARRAY
{
public:
	ARRAY();
	~ARRAY();

private:

};

ARRAY::ARRAY()
{
}

ARRAY::~ARRAY()
{
}
