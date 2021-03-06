#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <time.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstring>

// quote record - time, symbol, bid, ask, bsize, asize (datetime, string, double, double, int, int)
// trade record -  time, symbol, price, condition      (datetime, string, double, char) - condition could be empty.
// signal record - time, symbol, value, code           (datetime, string, double, int)

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)

struct datetime
{
  time_t time_since_epoch;
  unsigned short millisecs;

  void make_datetime(char* dt)
  {
    struct tm cdt = {0};
    strptime(dt, "%Y-%m-%d %H:%M:%S", &cdt);
    time_since_epoch = mktime(&cdt);
    char * dummy_str;
    dt = dt+20;
    millisecs = (unsigned short)strtol(dt, &dummy_str, 10);
  }

  // default constructor
  datetime()
  {
    millisecs = 0;
    time_since_epoch = 0;
  }

  friend std::ostream& operator<<(std::ostream& os, const datetime& t)
  {
    struct tm* cdt;
    cdt = localtime(&t.time_since_epoch);
    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", cdt);
    os << buff << "." << std::setfill ('0') << std::setw(3) << t.millisecs;
    return os;
  }
  // we are curently not considering day lights savings
  inline bool operator <(datetime& other)
  {
    if (this->time_since_epoch < other.time_since_epoch)
      return true;
    else if(this->time_since_epoch == other.time_since_epoch &&
	    this->millisecs < other.millisecs)
      return true;
    else
      return false;
  }

};

struct Messages
{
  char type;
  char sym[5];
  datetime dt;
};


struct Quote : Messages
{
  // char type; // Q
  double bid;
  double ask;
  int bid_size;
  int ask_size;

   friend std::ostream& operator<<(std::ostream& os, const Quote& t)
  {
    //    os << t.type <<
      os << t.dt  << ","
	 << t.sym << ","
	 << std::fixed << std::showpoint
	 << std::setprecision(4)
	 << t.bid << ","
	 << std::fixed << std::showpoint
	 << std::setprecision(4)
	 << t.ask << ","
	 << t.bid_size << ","
	 << t.ask_size << std::endl;

    return os;
  }

  Quote(char* instr, char* date_time, char * bid_price,
	char* ask_price, char* bid_qty, char* ask_qty)
  {
    type = 'Q';

    strcpy(sym, instr);

    dt.make_datetime(date_time);

    char * dummy_str;
    bid = strtod(bid_price, &dummy_str);
    ask = strtod(ask_price, &dummy_str);

    bid_size = strtol(bid_qty, &dummy_str, 10);
    ask_size = strtol(ask_qty, &dummy_str, 10);
  }

};

struct Trade : Messages
{
  // char type; // T
  double price;
  char condition; // optional

  Trade(char* instr, char* date_time, char * pr,
	char cond)
  {
    type = 'T';

    strcpy(sym, instr);

    dt.make_datetime(date_time);

    char * dummy_str;
    price = strtod(pr, &dummy_str);
    condition = cond;
  }

  friend std::ostream& operator<<(std::ostream& os, const Trade& t)
  {
    if (t.condition != 'Z')
      {
	os << t.dt << ","
	   << t.sym << ","
	   << std::fixed << std::showpoint
	   << std::setprecision(4)
	   << t.price << ","
	   << t.condition << std::endl;
      }
    else
      {
	os << t.dt << ","
	   << t.sym << ","
	   << std::fixed << std::showpoint
	   << std::setprecision(4)
	   << t.price << ","
	   << std::endl;
      }

    return os;
  }

};

struct Signal : Messages
{
  // char type; // S
  double value;
  int code;

  Signal(char* instr, char* date_time, char * val,
	 char* scode)
  {
    type = 'S';

    strcpy(sym, instr);

    dt.make_datetime(date_time);

    char * dummy_str;
    value = strtod(val, &dummy_str);
    code = strtol(scode, &dummy_str, 10);
  }

  friend std::ostream& operator<<(std::ostream& os,const Signal& t)
  {
    os << t.dt << ","
       << t.sym << ","
       << std::fixed << std::showpoint
       << std::setprecision(4)
       << t.value << ","
       <<  t.code << std::endl;
    return os;
  }
};

#pragma pack(pop)
#endif
