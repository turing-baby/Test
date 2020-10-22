#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    system("mplayer -slave -quiet 1.avi &");


    while(1)
    {

        system("killall -SIGSTOP mplayer");
        printf("暂停 3 秒\n");
        sleep(3);


        system("killall -SIGCONT mplayer");
        printf("播放 3 秒\n");

        sleep(3);


    }


    

    return 0;
}

