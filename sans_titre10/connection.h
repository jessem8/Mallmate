#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase> // Include for QSqlDatabase

class Connection
{
public:
    Connection(); // Constructor declaration
    bool createconnect(); // Method declaration for connecting to the database

private:
    QSqlDatabase db; // Declare the database object here
};

#endif // CONNECTION_H
