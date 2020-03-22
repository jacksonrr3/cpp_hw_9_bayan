#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
//#include <boost/filesystem/fstream.hpp>
//#include <boost/iostreams/device/file.hpp>
#include <fstream>

using namespace boost::filesystem;

class FileDescriptor {
	boost::filesystem::path _path;
	std::vector<std::size_t> _hash;
	std::size_t _file_size;
	std::size_t _offset = 0;
	std::size_t _buff_size;
	bool _is_duplicate = false;

public:
	FileDescriptor(const std::string& s, std::size_t block_size):
		_path(s),
		_buff_size(block_size)
	{};

	bool open_file() {
		bool res = true;
		boost::system::error_code ec;
		boost::uintmax_t fs = boost::filesystem::file_size(_path, ec);
		if (ec) {
			res = false;
		}
		else {
			_file_size = fs;
			if (fs % _buff_size) { ++_file_size; }
		}

		return res;
	}

	std::string read_file() {
		std::string buf(_buff_size, '0');
		std::ifstream _fstr;
		_fstr.rdbuf()->pubsetbuf(0, 0);
		_fstr.open(_path.string());
		_fstr.seekg(_offset, std::ios::beg);
		_fstr.read(&buf[0], _buff_size);
		_offset += _buff_size;
		return buf;
	}

	void add_hash(std::size_t h) {
		_hash.push_back(h);
	}

	bool is_end() {
		return _offset >= _file_size;
	}

	std::size_t get_size() const {
		return _file_size;
	}

	void set_duplicate(bool b) {
		_is_duplicate = b;
	}

	bool is_duplicate() const {
		return _is_duplicate;
	}

	friend bool operator<(const FileDescriptor& first, const FileDescriptor& second);
	friend bool operator==(const FileDescriptor& first, const FileDescriptor& second);
};

bool operator<(const FileDescriptor& first, const FileDescriptor& second) {
	return first._hash < second._hash;
}

bool operator==(const FileDescriptor& first, const FileDescriptor& second) {
	return (first._hash == second._hash) && (first._file_size == second._file_size);
}
