#include "parser.hpp"

#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>


// constructor
parser::parser(const char* filepath):
  _filepath(filepath),
  _filedesc(-1),
  _ptr(nullptr),
  _len(0)
{
  _filedesc = fd_open(filepath, _ptr, _len);
  parse(_ptr, _len);
}
//destructor
parser::~parser()
{
  fd_close();
}

void parser::parse(char* temp, size_t len)
{
  auto parse_start = std::chrono::system_clock::now();

  int line_len = 100000; // change it with INT_MAX
  for (long int i = 0; i < len && line_len > 0;)
    {
      char *p = (char*)rawmemchr(temp, '\n');
      line_len = p - temp;

      char buffer[line_len+1];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, temp, line_len);

      convert_to_message_type(buffer);

      i += line_len + 1;
      temp = p + 1;
    }

  //closes all the write filedesc
  close_write_filedesc();
  auto parse_end = std::chrono::system_clock::now();
  auto parse_elapsed = parse_end - parse_start;

  std::cout << "Parse (reading from csv and writing to binary file) time duration(in microsecs): "
	    << std::chrono::duration_cast<std::chrono::microseconds>(parse_elapsed).count()
	    << std::endl;

  // reads file from the diretory

  find_dat_files_from_current_dir();

  auto binary_start = std::chrono::system_clock::now();
  read_binary_files();
  auto binary_end = std::chrono::system_clock::now();

  auto binary_elapsed = binary_end - binary_start;
  std::cout << "Binary file reading and deserialization time duration(in microsecs): "
	    << std::chrono::duration_cast<std::chrono::microseconds>(binary_elapsed).count()
	    << std::endl;

  auto write_start = std::chrono::system_clock::now();
  sort_and_write();
  auto write_end = std::chrono::system_clock::now();
  auto write_elapsed = write_end - write_start;
  std::cout << "Sort and Write to csv time duration(in microsecs): "
	    << std::chrono::duration_cast<std::chrono::microseconds>(write_elapsed).count()
	    << std::endl;

}


void parser::convert_to_message_type(char* buffer)
{
    int field_count = 1;
    char* pch;
    char* date_time = nullptr;
    char* instr = nullptr;
    char* bid_price_value = nullptr;
    char* ask_conditon_code = nullptr;
    char* b_size = nullptr;
    char* a_size = nullptr;

    pch = strtok (buffer,",");
    while (pch != nullptr)
      {
	switch(field_count)
	  {
	  case 1:
	    date_time =  pch;
	    break;
	  case 2:
	    instr = pch;
	    break;
	  case 3:
	    bid_price_value = pch;
	    break;
	  case 4:
	    ask_conditon_code = pch;
	    break;
	  case 5:
	    b_size = pch;
	    break;
	  case 6:
	    a_size = pch;
	    break;
	  default:
	    break;
	    std::cout << "Unknown behaviour!. some problem with input file!" << std::endl;
	  }
	++field_count;
	pch = strtok (NULL, ",");
      }

    if (field_count > 5)
      {
	Quote q(instr, date_time, bid_price_value, ask_conditon_code, b_size, a_size);
	write_to_file(q, instr);
      }
    else
      {

	if (!ask_conditon_code || (ask_conditon_code[0] >= 'a' && ask_conditon_code[0] <= 'z') )
	  {
	    if (ask_conditon_code)
	      {
		Trade t(instr, date_time,bid_price_value, ask_conditon_code[0]);
		write_to_file(t, instr);
	      }
	    else
	      {
		Trade t(instr, date_time,bid_price_value, 'Z');
		write_to_file(t, instr);
	      }
	  }
	else
	  {
	    Signal s(instr, date_time,bid_price_value, ask_conditon_code);
	    write_to_file(s, instr);
	  }
      }
}

template <typename T>
void parser::write_to_file(const T& x ,const char* instr )
{
  auto it = _umap.find(instr);
  if (it != _umap.end())
    {
      write(it->second, &x,sizeof(x)) ;
    }
  else
    {
      char fname[8];
      strcpy(fname, instr);
      strcat(fname ,".dat");
      int filedesc = open(fname, (O_WRONLY | O_CREAT), (mode_t)0666 );

      if (filedesc < 0)
	{
	std::cerr << "file could not be created" << std::endl;
	return;
      }

      write(filedesc, &x,sizeof(x)) ;
      _umap.emplace(instr, filedesc);
    }
}

void parser::close_write_filedesc()
{
  for (auto it : _umap)
    close(it.second);
}


void parser::find_dat_files_from_current_dir()
{
  DIR* dirp = opendir(".");
  if (dirp == NULL)
    {
      std::cerr << "could not open current directory" << std::endl;
      return;
    }

  char name[10];
  strcpy(name, ".dat");

  int len = strlen(name);
  struct dirent *dp;
  std::vector<std::string> files;

  while ((dp = readdir(dirp)) != NULL)
    {
      files.push_back(dp->d_name);
    }
  (void)closedir(dirp);

  for (auto it = files.begin() ; it < files.end(); ++it)
    {
      std::size_t found = (*it).find(name);
      if (found != std::string::npos)
	{
	  std ::cout<< *it << std::endl;
	  _file_paths.push_back(*it);
	}
    }
}

void parser::read_binary_files()
{
   for (auto it = _file_paths.begin() ; it < _file_paths.end(); ++it)
     {
       std::string str = *it;
       int fd = open(str.c_str(), O_RDONLY, (mode_t)0600);
       if (fd == -1)
	 {
	   std::cerr << "Error opening binary file for reading\n";
	   return;
	 }

       struct stat fileInfo = {0};
       if (fstat(fd, &fileInfo) == -1)
	 {
	   std::cerr << "Error getting the binary file size\n";
	   return;
	 }

       if (fileInfo.st_size == 0)
	 {
	   std::cerr << "Error: binary file is empty, nothing to do\n";
	   return;
	 }

       std::cout << "Binary file size is :" << (intmax_t)fileInfo.st_size << std::endl;
       char* map= (char*)mmap(0, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

       if (map == MAP_FAILED)
	 {
	   close(fd);
	   std::cerr << "Error mmapping the binary file\n";
	   return;
	 }

       int len = fileInfo.st_size;

       message_processor(map, len);

       close(fd);
     }
}

void parser::message_processor(char* msg,  size_t len)
{
  size_t consumed_byte  = 0;
  while (len - consumed_byte)
    {
      char msg_type = *(msg + consumed_byte);
      switch(msg_type)
	{
	case 'Q':
	  {
	    _msgs.push_back(reinterpret_cast<Quote*>(msg+consumed_byte));
	    consumed_byte += sizeof(Quote);
	    break;
	  }
	case 'T':
	  {
	    _msgs.push_back(reinterpret_cast<Trade*>(msg+consumed_byte));
	    consumed_byte += sizeof(Trade);
	    break;
	  }
	case 'S':
	  {
	    _msgs.push_back(reinterpret_cast<Signal*>(msg+consumed_byte));
	    consumed_byte += sizeof(Signal);
	    break;
	  }
	default:
	  std::cerr << "Unknown message type: "<< msg_type << " ignoring packet!!"<< std::endl;
	  return;
	}
    }
}

void parser::sort_and_write()
{

  auto sort_start = std::chrono::system_clock::now();

  std::sort(_msgs.begin(), _msgs.end(), [](Messages* first, Messages* second) {
      return  first->dt < second->dt ;
    });

  auto sort_end = std::chrono::system_clock::now();

  auto sort_elapsed = sort_end - sort_start;
  std::cout << "Sort duration time duration(in microsecs): "
	    << std::chrono::duration_cast<std::chrono::microseconds>(sort_elapsed).count()
	    << std::endl;

  char dest_path[30];
  strcpy(dest_path, "copy_");
  strcat(dest_path, _filepath);

  std::ofstream outfile;
  outfile.open (dest_path);

  size_t count = 0;
  for (auto it = _msgs.begin() ; it < _msgs.end(); ++it)
     {
       if  ((*it)->type == 'Q')
	 outfile << *(static_cast<Quote*>(*it));
       else if ((*it)->type == 'T')
	 outfile << *(static_cast<Trade*>(*it));
       else
	 outfile << *(static_cast<Signal*>(*it));
       ++count;
     }
  std::cout << "Number of messages wrote in " << dest_path << ": " << count << std::endl;
  outfile.close();

}

int parser::fd_open(const char* filepath, char* read_buffer, size_t& len)
{
  int fd = open(filepath, O_RDONLY, (mode_t)0600);
  if (fd == -1)
    {
      std::cerr << "Error opening file for reading\n";
      return -1;
    }

  struct stat fileInfo = {0};
  if (fstat(fd, &fileInfo) == -1)
    {
      std::cerr << "Error getting the file size\n";
      return -1;
    }

  if (fileInfo.st_size == 0)
    {
      std::cerr << "Error: File is empty, nothing to do\n";
      return -1;
    }

  std::cout << "File size is :" << (intmax_t)fileInfo.st_size << std::endl;
  // TODO this need to be readbuffer
  _ptr = (char*)mmap(0, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (read_buffer == MAP_FAILED)
    {
      close(fd);
      std::cerr << "Error mmapping the file\n";
      return -1;
    }

  len = fileInfo.st_size;
  return fd;
}

void parser::fd_close()
{
  if (munmap(_ptr, _len) == -1)
    {
      close(_filedesc);
      std::cerr << "Error un-mmapping the file\n";
      return;
    }
  // Un-mmaping doesn't close the file, so we still need to do that.
  close(_filedesc);
}
