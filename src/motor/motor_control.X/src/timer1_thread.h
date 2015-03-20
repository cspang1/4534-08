typedef struct __timer1_thread_struct {
    // "persistent" data for this "lthread" would go here
    unsigned int msgcount;
    unsigned char ctrl[2];
    char quadpos[32];
    char prevPos[2];
    char current_position;
    unsigned char direction;
    unsigned int cmDist;
    unsigned int curMove;
    BOOL turning;
    BOOL distLim;

} timer1_thread_struct;

void init_timer1_lthread(timer1_thread_struct *);
int timer1_lthread(timer1_thread_struct *,int,int,unsigned char*);