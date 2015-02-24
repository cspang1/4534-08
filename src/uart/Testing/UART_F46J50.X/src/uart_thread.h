typedef struct __uart_thread_struct {
    // "persistent" data for this "lthread" would go here
    unsigned char data[10];
} uart_thread_struct;

void init_uart_lthread(uart_thread_struct *);
int uart_lthread(uart_thread_struct *,int,int,unsigned char*);
