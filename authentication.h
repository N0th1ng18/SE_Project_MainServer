#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H


class Authentication
{
public:
    Authentication();
    ~Authentication();

    void addConn();
    void removeConn();
    void checkConn();
    short getConnCode();

private:


};

#endif // AUTHENTICATION_H
