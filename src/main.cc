#include "gmyqq.h"
#define USERNAME 1147071944
int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        if(!strcmp(argv[1],"--nogui"))
        {
            //myqq(argc,argv);
        }
        else if(!strcmp(argv[1],"--help"))
        {
            //gMystar::help();
        }
        else if(!strcmp(argv[1],"--test"))
        {
        }
        else if(!strcmp(argv[1],"-s"))
        {
        }
        else
        {
            //cout<<"usage: gmyqq--help for more details"<<endl;
        }
    }
    else
    {
        Gtk::Main kit(argc, argv);
        gMyQQ gmyqq;
        kit.run();
    }
    return 0;
}
