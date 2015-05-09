
#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H

#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
/* GPIO Class
 * Purpose: Each object instatiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 */
class GPIOClass
{
public:
	GPIOClass();
	GPIOClass(string gnum);
	~GPIOClass();
    bool setdir_gpio(string dir);
    bool setval_gpio(string val);
    bool getval_gpio(string& val);
    string get_gpionum();
private:
    int export_gpio();
	int unexport_gpio();
	
	int valuefd;
	int directionfd;
	int exportfd;
	int unexportfd;
	string gpionum;
    bool mIsExported = false;
	
    const std::string & getLoggerPrefix();
};

#endif
