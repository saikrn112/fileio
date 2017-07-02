#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <unordered_map>
#include <vector>
#include <string>

#include "../include/messages.hpp"

class parser
{
public:
  parser(const char* ptr);
  ~parser();

private:
  void parse(char* temp, size_t len);
  int fd_open(const char* filepath, char* read_buffer, size_t& len);
  void fd_close();

  void convert_to_message_type(char* buffer);

  template <typename T>
  void write_to_file(const T& x,const char* instr );

  void close_write_filedesc();
  void find_dat_files_from_current_dir();

  void read_binary_files();

  void message_processor(char* msg, size_t len);
  void sort_and_write();

  const char* _filepath;
  int _filedesc;
  char* _ptr;
  size_t _len;
  std::unordered_map<std::string, int> _umap;
  std::vector<std::string> _file_paths;
  std::vector<Messages*> _msgs;
};

#endif
