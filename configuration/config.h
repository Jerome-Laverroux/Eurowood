#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>


class Config: public QObject
{
    Q_OBJECT
public:
    Config();
   Q_INVOKABLE bool isAndro();
};

#endif // CONFIG_H
