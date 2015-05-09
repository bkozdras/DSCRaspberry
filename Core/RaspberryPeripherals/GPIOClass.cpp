
#include "GPIOClass.hpp"
#include "../Utilities/Logger.hpp"

using namespace std;

GPIOClass::GPIOClass():valuefd(-1),directionfd(-1),exportfd(-1),unexportfd(-1),gpionum("4")
{
        //GPIO4 is default
	this->export_gpio();
}

GPIOClass::GPIOClass(string gnum):valuefd(-1),directionfd(-1),exportfd(-1),unexportfd(-1),gpionum(gnum)
{
	//Instatiate GPIOClass object for GPIO pin number "gnum"
	this->export_gpio();
}

GPIOClass::~GPIOClass()
{
    if (mIsExported)
    {
        this->unexport_gpio();
    }
}


int GPIOClass::export_gpio()
{
	int statusVal = -1;
	string exportStr = "/sys/class/gpio/export";
	this->exportfd = statusVal = open(exportStr.c_str(),  O_WRONLY|O_SYNC);
	if (statusVal < 0){
        Logger::error("%s: Could not open SYSFS GPIO %s export device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return statusVal;
	}
	
	stringstream ss;
	ss << this->gpionum;
	string numStr = ss.str();
	statusVal = write(this->exportfd, numStr.c_str(), numStr.length());
	if (statusVal < 0){
        //Logger::error("%s: Could not write to SYSFS GPIO %s export device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return statusVal;
	}
	
	statusVal = close(this->exportfd);
	if (statusVal < 0){
        Logger::error("%s: Could not close SYSFS GPIO %s export device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return statusVal;
	}

    mIsExported = true;

    return statusVal;
}

int GPIOClass::unexport_gpio()
{
	int statusVal = -1;
	string unexportStr = "/sys/class/gpio/unexport";
	this->unexportfd = statusVal = open(unexportStr.c_str(),  O_WRONLY|O_SYNC);
	if (statusVal < 0){
        Logger::error("%s: Could not open SYSFS GPIO %s unexport device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return statusVal;
	}

	stringstream ss;
	ss << this->gpionum;
	string numStr = ss.str();
	statusVal = write(this->unexportfd, numStr.c_str(), numStr.length());
	if (statusVal < 0){
        Logger::error("%s: Could not write to SYSFS GPIO %s unexport device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return statusVal;
	}
	
	statusVal = close(this->unexportfd);
	if (statusVal < 0){
        Logger::error("%s: Could not close SYSFS GPIO %s unexport device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return statusVal;
	}
	
	return statusVal;
}

bool GPIOClass::setdir_gpio(string dir)
{
	int statusVal = -1;
	string setdirStr = std::string("/sys/class/gpio/gpio") + this->gpionum + std::string("/direction");
	
	
	this->directionfd = statusVal = open(setdirStr.c_str(), O_WRONLY|O_SYNC); // open direction file for gpio
		if (statusVal < 0){
            Logger::error("%s: Could not open SYSFS GPIO %s direction device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
            return false;
		}
		
	if (dir.compare("in") != 0 && dir.compare("out") != 0 ) {
        Logger::error("%s: Invalid SYSFS GPIO %s direction value.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return false;
	}
		
	statusVal = write(this->directionfd, dir.c_str(), dir.length());
	if (statusVal < 0){
        Logger::error("%s: Could not write to SYSFS GPIO %s direction device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return false;
	}
	
	statusVal = close(this->directionfd);
	if (statusVal < 0){
        Logger::error("%s: Could not close SYSFS GPIO %s direction device.", getLoggerPrefix().c_str(), this->gpionum.c_str());
        return false;
	}

	    return true;
}


bool GPIOClass::setval_gpio(string val)
{

    int statusVal = -1;
	string setValStr = std::string("/sys/class/gpio/gpio") + this->gpionum + std::string("/value");
	
	this->valuefd = statusVal = open(setValStr.c_str(), O_WRONLY|O_SYNC);
	if (statusVal < 0){
            Logger::error("%s: Could open SYSFS GPIO %s value device.", getLoggerPrefix().c_str(), val.c_str());
            return false;
	}
		
	if (val.compare("1") != 0 && val.compare("0") != 0 ) {
        Logger::error("%s: Invalid SYSFS GPIO %s value.", getLoggerPrefix().c_str(), val.c_str());
        return false;
	}
		
	statusVal = write(this->valuefd, val.c_str(), val.length());
	if (statusVal < 0){
        Logger::error("%s: Could not write to SYSFS GPIO %s value device.", getLoggerPrefix().c_str(), val.c_str());
        return false;
	}
	
	statusVal = close(this->valuefd);
	if (statusVal < 0){
        Logger::error("%s: Could not close SYSFS GPIO %s value device.", getLoggerPrefix().c_str(), val.c_str());
        return false;
	}

    return true;
}


bool GPIOClass::getval_gpio(string& val){

	string getValStr = std::string("/sys/class/gpio/gpio") + this->gpionum + std::string("/value");
	char buff[10];
	int statusVal = -1;
	this->valuefd = statusVal = open(getValStr.c_str(), O_RDONLY|O_SYNC);
	if (statusVal < 0){
            Logger::error("%s: Could not open SYSFS GPIO %s value device.", getLoggerPrefix().c_str(), val.c_str());
            return false;
	}

	statusVal = read(this->valuefd, &buff, 1);
	if (statusVal < 0){
        Logger::error("%s: Could not read SYSFS GPIO %s value device.", getLoggerPrefix().c_str(), val.c_str());
        return false;
	}
	
	buff[1]='\0';
	
	val = string(buff);
	
	if (val.compare("1") != 0 && val.compare("0") != 0 ) {
        Logger::error("%s: Invalid SYSFS GPIO %s read value.", getLoggerPrefix().c_str(), val.c_str());
        return false;
	}
	
	statusVal = close(this->valuefd);
	if (statusVal < 0){
		Logger::error("%s: Could not close SYSFS GPIO %s value device.", getLoggerPrefix().c_str(), val.c_str());
        return false;
	}

    return true;
}


string GPIOClass::get_gpionum(){

return this->gpionum;

}

const std::string & GPIOClass::getLoggerPrefix()
{
    static std::string loggerPrefix("GPIOHardwareConnector");
    return loggerPrefix;
}
