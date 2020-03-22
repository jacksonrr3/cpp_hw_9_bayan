#pragma once

#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
//#include <boost/format.hpp>

#include "hash.h"

using namespace boost::program_options;

class Options {
	options_description _desc{ "Options" };
	variables_map _vm;
	std::string _path;
	std::string _type;
	std::vector<std::string> _files;
	std::size_t _block_size;
	std::string _hash_type;
	std::shared_ptr<Hash> _hash;

public:

	Options() {
		_desc.add_options()
			("help", "Help screen")
			("path", value<std::string>(), "path to files")
			("type", value<std::string>(), "file type")
			("files", value<std::vector<std::string>>()->
				multitoken()->zero_tokens()->composing(), "file names for compare")
			("size", value<std::size_t>()->default_value(3), "block size")
			("hash", value<std::string>()->default_value("boost_hash"), "hash type (boost_hash or boost_crc32)");
		
	}

	void parse_comm_line_opt(int argc, const char* argv[]) {
		store(parse_command_line(argc, argv, _desc), _vm);
		notify(_vm);

		if (_vm.count("help"))
			std::cout << _desc << '\n';

		if (_vm.count("size")) {
			_block_size = _vm["size"].as<std::size_t>();
		}

		if (_vm.count("hash")) {
			Hash_factory hf;
			_hash = hf.make_hash_obj(_vm["hash"].as<std::string>());
			_hash_type = _vm["hash"].as<std::string>();
		}

		if (_vm.count("path")) {
			_path = _vm["path"].as<std::string>();
		}

		if (_vm.count("type")) {
			_type = "." + _vm["type"].as<std::string>();
		}

		if (_vm.count("files")) {
			_files = _vm["files"].as<std::vector<std::string>>();
			for (auto& f : _files) {
				f = _path + f + _type;
			}
		}

		if ((_files.size() == 0)) {
			std::cout << "No files for compare" << std::endl;
		}
	}

	std::size_t do_hash(const std::string& s) const {
		return _hash->count(s);
	}

	const std::vector<std::string>& files() const {
		return (const std::vector<std::string>&)_files;
	}

	std::size_t block_size() const {
		return _block_size;
	}

	friend class File_Handler;
};
