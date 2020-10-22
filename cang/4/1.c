#include "my.h"

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("1111");
        exit(1);
    }
    DIR * dp = opendir(argv[1]);
    struct dirent * de = NULL;
    while (1)
    {
        de = readdir(dp);
        if(de == NULL)
        {
            break ;
        }
        printf("%ld  ==   %s   \n",de->d_ino, de->d_name);
       
    }
    
    
    
    return 0;
}
