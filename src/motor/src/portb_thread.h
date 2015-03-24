typedef struct __portb_thread_struct {
    // "persistent" data for this "lthread" would go here
    unsigned char data[10];
    unsigned char dir;
    int msgcount;
    int dist;
} portb_thread_struct;

void init_portb_lthread(portb_thread_struct *);
int portb_lthread(portb_thread_struct *);
