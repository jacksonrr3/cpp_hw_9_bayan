#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fstream>

using namespace boost::filesystem;

class FileDescriptor {
	boost::filesystem::path _path;
	std::shared_ptr<ifstream> _fstream;
	std::vector<std::size_t> _hash;
	std::size_t _file_size;
	std::size_t _offset = 0;
	std::size_t _buff_size;
	bool _is_double = false;

public:
	FileDescriptor(const std::string& s, std::size_t block_size): 
		_path(s),
		_fstream(std::make_shared<ifstream>(_path)),
		_buff_size(block_size) 
	{
	};

	~FileDescriptor() {
		_fstream->close();
	}

	void open_file() {
		boost::system::error_code ec;
		boost::uintmax_t fs = file_size(_path, ec);
		if (ec) {
			throw Bayan_exeption("wrong_file!");
		}
		else {
			_file_size = fs / _buff_size;
			if (fs % _buff_size) { ++_file_size; }
		}
		//_fstream->open(_path); 
	}

	std::string read_file() {
		std::string buf;
		buf.resize(_buff_size);
		_fstream->rdbuf()->pubsetbuf(0, 0);
		_fstream->read(&buf[0], _buff_size);
		++_offset;
		return buf;
	}

	void add_hash(std::size_t h) {
		_hash.push_back(h);
	}

	bool is_end() {
		return _offset == _file_size;
	}

	std::size_t get_size() const {
		return _file_size;
	}

	void set_double(bool b) {
		_is_double = b;
	}

	bool is_double() const {
		return _is_double;
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

