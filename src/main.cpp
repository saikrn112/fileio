#include "parser.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{
  auto start = std::chrono::system_clock::now();
  try
    {
      if (argc != 2)
      	{
      	  std::cerr << "Usage: serialize <filename.csv>\n";
      	  return 1;
      	}
      const char *filepath = argv[1];
      parser p(filepath);
    }
  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

  auto end = std::chrono::system_clock::now();
  auto elapsed = end - start;

  std::cout << "Total time duration(in microsecs): "
	    << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
	    << std::endl;

  return 0;
}
