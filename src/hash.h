#pragma once
#include <boost/functional/hash.hpp>
#include <boost/container/vector.hpp>
#include <boost/crc.hpp>
#include <string>
#include <memory>


class Bayan_exeption : public boost::exception {
	std::string _message;
public:
	Bayan_exeption(const std::string& s):_message(s) {}

	const char* what() const noexcept { 
		return _message.c_str(); 
	}
};

class Hash {
public:
	Hash() = default;
	virtual ~Hash() = default;

	virtual std::size_t count(const std::string&) const = 0;
};


class Boost_Hash : public Hash {
	virtual std::size_t count(const std::string& s) const override
	{
		boost::hash<const std::string> h;
		return h(s);
	}
};

class Boost_Crc_32 : public Hash {
	virtual std::size_t count(const std::string& s) const override
	{
		boost::crc_32_type crc_hash;
		crc_hash.process_bytes(s.data(), s.size());
		return crc_hash.checksum();
	}
};

class Hash_factory {
public:
	std::shared_ptr<Hash> make_hash_obj(const std::string& s) {
		if (s == "boost_hash") {
			return std::make_shared<Boost_Hash>();
		}
		else if (s == "boost_crc32") {
			return std::make_shared<Boost_Crc_32>();
		}
		else {
			throw(Bayan_exeption("wrong hash name (Hash_factory::make_hash_obj)"));
			return 0;
		}
	}

};
