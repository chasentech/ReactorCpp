#include <iostream>
#include "connection.h"

using namespace std;


int main()
{
    /*
    //test data_to_buff
    char temp[100] = {0};
    data_to_buff(temp, 100, 1);
    printf("%d ", temp[0]);
    printf("%s\n", temp + 1);
    */

    Connection connection("127.0.0.1", 8000);

    connection.connectServer(10000);
    connection.connectLoop();

    return 0;
}
