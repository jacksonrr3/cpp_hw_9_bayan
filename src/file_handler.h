#pragma once

#include <utility>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "hash.h"
#include "options.h"
#include "filedesc.h"


class File_Handler {
	std::unordered_map<std::string, FileDescriptor> _files;
	const Options& _opt;
	std::set<std::string> _res;

public:
	File_Handler(const Options& opt) :_opt(opt) {}

	void run() {
		prepare();
		while (_files.size() > 1) {
			read_block();
			do_compare();
		}
		print_res();
	}

private:
	void print_res() {
		for (auto& s : _res) {
			std::cout << absolute(path(s)) << std::endl;
		}
	}

	void prepare() {
		std::for_each(_opt._files.begin(), _opt._files.end(), [this](auto str) {
			auto it = _files.emplace(str, FileDescriptor(str, _opt.block_size()));
			if (it.second) {
				it.first->second.open_file();
			}
			});
		
		//проверка на совпадение размера файлов. Файлы с уникальным размером удаляются из списка поиска. 
		for (auto it = _files.begin(); it != _files.end(); ) {
			if (!it->second.is_duplicate()) {
				for (auto jt = std::next(it, 1); jt != _files.end(); jt++) {
					if (it->second.get_size() == jt->second.get_size()) {
						it->second.set_duplicate(true);
						jt->second.set_duplicate(true);
					}

				}
				if (!it->second.is_duplicate()) {
					it = _files.erase(it);
				}
				else {
					it++;
				}
			}
			else {
				it++;
			}
		}
	}

	void read_block() {
		for (auto it = _files.begin(); it != _files.end();) {
			if (it->second.is_end()) {
				if (it->second.get_size()) {
					_res.emplace(it->first);
				}
				it = _files.erase(it);
			}
			else {
				it->second.set_duplicate(false);
				it->second.add_hash(_opt.do_hash(it->second.read_file()));
				it++;
			}
		}
	}

	void do_compare() {
		auto it = _files.begin();
		for (; it != _files.end(); ) {
			if (!it->second.is_duplicate()) {
				auto jt = it;
				++jt;
				for (; jt != _files.end(); jt++) {
					if (it->second == jt->second) {
						it->second.set_duplicate(true);
						jt->second.set_duplicate(true);
					}
				}
				if (!it->second.is_duplicate()) {
					it = _files.erase(it);
					continue;
				}
			}
			it++;
		}
	}
};




