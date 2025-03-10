#ifndef __DEFINES_HPP
#define __DEFINES_HPP
//----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
//#include <experimental/filesystem>
#include <mutex>  
#include <queue>
#include <vector>  
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <future>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <netdb.h>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>
#include "../Module/Print.hpp"
#include "../nlohmann/json.hpp"
#include <curl/curl.h>
#include <termios.h>
#include <fcntl.h>
#include <cstddef>
#include <mutex>
#include <stdarg.h>
//----------------------------------------------------------
using namespace std;
//----------------------------------------------------------
vector<string> DEFINE_split(const string &s, char delimiter);
//----------------------------------------------------------
#endif
