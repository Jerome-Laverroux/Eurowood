#include "config.h"

Config::Config()
{

}

bool Config::isAndro()
{
    bool out=false;

#ifdef Q_OS_ANDROID
    out=true;
#endif
    return out;
}
