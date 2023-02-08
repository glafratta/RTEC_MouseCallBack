#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>

struct Mousecallback {
    virtual void hasData(int x, int y, bool left, bool middle, bool right) = 0;
};

//find input devices connected: cat /proc/bus/input/devices and get event handler
//remember to run with sudo! 

class Mouse {
private:
    const char *pDevice = "/dev/input/mice";
    int fd; //file descriptor
    unsigned char data[3];
    std::thread * t;
    bool running =0;
    Mousecallback *mousecallback;
    static void readData(Mouse * m);
public:
    void registerCallback(Mousecallback* mc);
    void start();
    void stop();

};

void Mouse::start(){
    running =1;
    fd = open(pDevice, O_RDWR);
    printf("fd = %i\n", fd);

    if(fd == -1)
    {
        printf("ERROR Opening %s\n", pDevice);
        return;
    }
    if (nullptr != t){ //check not already running
        return;
    }
    t = new std::thread(Mouse::readData, this);
}

void Mouse::stop(){
    running =0;
    if (nullptr != t){
        t->join();
        delete t;
        t= nullptr;
    }
    printf("stop\n");

}

void Mouse::registerCallback(Mousecallback * mc){
    mousecallback = mc;
}

void Mouse::readData(Mouse * m){
    int left, middle, right; 
    signed char x,y;
    while (m->running){
        int bytes = read(m->fd, m->data, sizeof(m->data)); //reads from filedescriptor into the data buffer and parses the data into chunks of a certain size
        if (bytes >0){
            left = m->data[0] & 0x1;
            right = m->data[0] & 0x2;
            middle = m->data[0] & 0x4;
            x = m->data[1];
            y = m->data[2];
            m->mousecallback->hasData(x, y, left, middle, right);
        }
        // else{
        //     printf("error reading bytes\n");
        // }
    }
}
struct CB :public Mousecallback{
    void hasData(int x, int y, bool left, bool middle, bool right){
        printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
    }
};

int main(){
Mouse mouse;
CB mcb;
mouse.registerCallback(&mcb);
mouse.start();
do {

}while (!getchar());
mouse.stop();
}