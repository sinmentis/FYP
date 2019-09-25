/******************************************************************************/


/******************************************************************************/
#include "mbed.h"
#include "EthernetInterface.h"
#include "UDPSocket.h"

#define DEBUG_MODE 0
#define PAYLOAD_LEN 10
#define UART_TIMEOUT 1
/*###########################   CONFIGURATION ################################*/

// ###########################################   Ethernet Config
EthernetInterface net;
SocketAddress rd_addr;
UDPSocket sock_in;
UDPSocket sock_out;

#define MASTER_IP "192.168.1.26"
#define PC_IP     "192.168.1.25"
#define NETMASK   "255.255.255.0"
#define GATEWAY   "192.168.1.1" 

// ############################################  uart(TX, RX)
Serial pc(SERIAL_TX, SERIAL_RX);

// ############################################  Global Variable
int Button_Flag;
char UDP_message_buffer[80];

// ###########################################   FreeRTOS
EventQueue queue;
Timeout flipper1;
Timeout flipper2;
Ticker t;

typedef struct  __attribute__ ((packed)) UART_payload_t{
    int board_type : 4;
    int board_num : 4;
    uint8_t board_add;
    uint8_t state;
} UART_payload;

union UART_data{
    UART_payload UART_data;
    char UART_data_buffer[3];
};

union UDP_data{
    UART_payload UART_data[PAYLOAD_LEN];
    char UART_data_buffer[PAYLOAD_LEN * 3]; 
};

union UART_data UART_in_buffer;  // this is where the UART interrupt writes to
union UDP_data UDP_in_buffer;    // this is where the UDP writes to: UDP_in_buffer.UART_data_buffer

union UART_data UART_out_buffer; // this is what the UART sends

UART_payload UDP_payload[PAYLOAD_LEN]; // this is what UDP sends

volatile int current_index = 0;


/*###########################   Support  Function     ########################*/
class MySerial: public RawSerial
{
public:

  //MySerial(PinName tx, PinName rx, const char *name=NULL, int baud=MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) : Serial(tx, rx, name, baud) {};
  // reads a character, waiting for up to timeout ms
  // returns -1 in case of timeout
  // timeout: timeout in ms, -1 for infinite
    MySerial(PinName tx, PinName rx):RawSerial(tx,rx){};
    int getc( int timeout = -1 );
    int puts (const char *str);
};

int MySerial::getc( int timeout )
{
    // if infinite timeout or we have a character, call base getc()
    if ( timeout == -1 || readable() )
        return RawSerial::getc();

    // no character yet
    bool has_data = false;
    // count elapsed time
    Timer timer;
    timer.start();
    // loop until we have data or timeout elapses  
    while ( !has_data && timer.read_ms() < timeout ) {
        // wait a short time
        wait_ms(1);
        // check again
        has_data = readable();
    }
    // do we have anything?
    if ( has_data )
        // yes, read it
        return RawSerial::getc();
    else
        // no, timed out
        return -1;
};
int MySerial::puts(const char *str)
{
    return RawSerial::puts(str);
}

MySerial uart_led(UART3_TX, UART3_RX);  
MySerial uart_but(UART7_TX, UART7_RX);  
MySerial uart_oth(UART8_TX, UART8_RX); 

void receive_UDP()
{
    sock_in.set_timeout(3);
    int ret = sock_in.recvfrom(&rd_addr ,UDP_in_buffer.UART_data_buffer, sizeof(UDP_in_buffer.UART_data_buffer));
    if (ret != -3001) {
        if (DEBUG_MODE){
            printf("Packet from \"%s\": ret:%d %s\n", rd_addr.get_ip_address(),ret, UDP_in_buffer.UART_data_buffer);
        }
      
        for(int i = 0; i < PAYLOAD_LEN; i++)
        {
            // assigns the packet info from the packet in the payload into a new uart packet
            // iterating through all packets which have come through the payload
            // checkes that the payload is not null
                    
            UART_out_buffer.UART_data.board_type = UDP_in_buffer.UART_data[i].board_type;
            UART_out_buffer.UART_data.board_num = UDP_in_buffer.UART_data[i].board_num;
            UART_out_buffer.UART_data.board_add = UDP_in_buffer.UART_data[i].board_add;
            UART_out_buffer.UART_data.state = UDP_in_buffer.UART_data[i].state;
            if(DEBUG_MODE){
                printf("type: %d, num: %d, add: %d, state: %d", UART_out_buffer.UART_data.board_type,
                                                                UART_out_buffer.UART_data.board_num,
                                                                UART_out_buffer.UART_data.board_add,
                                                                UART_out_buffer.UART_data.state);  
            }
            // Pots, dials, 7 segs
            if (UART_out_buffer.UART_data.board_type == 0b0001 && UART_out_buffer.UART_data.board_num == 0b0001)
            {
                for (int x = 0; x < sizeof(UART_out_buffer.UART_data_buffer); x++)
                {
                    uart_led.putc(UART_out_buffer.UART_data_buffer[x]);
                }
            } else if (UART_out_buffer.UART_data.board_type == 0b0010 && UART_out_buffer.UART_data.board_num == 0b0001) {
                for (int x = 0; x < sizeof(UART_out_buffer.UART_data_buffer); x++)
                {
                    uart_but.putc(UART_out_buffer.UART_data_buffer[x]);
                }
            } else if (UART_out_buffer.UART_data.board_type == 0b0100 && UART_out_buffer.UART_data.board_num == 0b0001) {
                for (int x = 0; x < sizeof(UART_out_buffer.UART_data_buffer); x++)
                {
                    uart_oth.putc(UART_out_buffer.UART_data_buffer[x]);
                }
            }
        } 
    }
    memset(&UDP_in_buffer, 0, sizeof(UDP_in_buffer));
    
    
}

// WORKING :)
void send_UDP()
{
    if (current_index != 0) {
        int scount = sock_out.sendto(PC_IP,5005, UDP_payload, sizeof UDP_payload);
        if(DEBUG_MODE){
            pc.printf("sent %d [%.*s]\n", scount, UDP_payload);
        }
        current_index = 0;
        memset(UDP_payload, 0, sizeof(UDP_payload));
    }
}


// WORKING :)
void receive_led_UART()
{
    if (uart_led.readable()) {
        for (int x = 0; x < sizeof(UART_in_buffer.UART_data_buffer) + 1; x++)
        {
            UART_in_buffer.UART_data_buffer[x] = uart_led.getc(UART_TIMEOUT);
        }
        if(DEBUG_MODE) {    
            pc.printf("Got from LED UART address: %c (%d), state: %c (%d) \n", UART_in_buffer.UART_data.board_add, UART_in_buffer.UART_data.board_add, UART_in_buffer.UART_data.state, UART_in_buffer.UART_data.state);
        }
        UDP_payload[current_index] = UART_in_buffer.UART_data;
        current_index++;
        memset(&UART_in_buffer, 0, sizeof(UART_in_buffer));
    }
}

void receive_but_UART()
{    
    if (uart_but.readable()) {
        for (int x = 0; x < sizeof(UART_in_buffer.UART_data_buffer) + 1; x++)
        {
            UART_in_buffer.UART_data_buffer[x] = uart_but.getc(UART_TIMEOUT);
        }
        if(DEBUG_MODE) {
            pc.printf("Got from Button UART address: %c (%d), state: %c (%d) \n", UART_in_buffer.UART_data.board_add, UART_in_buffer.UART_data.board_add, UART_in_buffer.UART_data.state, UART_in_buffer.UART_data.state);    
        }
        UDP_payload[current_index] = UART_in_buffer.UART_data;
        current_index++;  
        memset(&UART_in_buffer, 0, sizeof(UART_in_buffer));  
    }    
}

void receive_oth_UART()
{    
    if (uart_oth.readable()) {
        for (int x = 0; x < sizeof(UART_in_buffer.UART_data_buffer) + 1; x++)
        {
            UART_in_buffer.UART_data_buffer[x] = uart_oth.getc(UART_TIMEOUT);
        }
        if(DEBUG_MODE) {
            pc.printf("Got from other UART address: %c (%d), state: %c (%d) \n", UART_in_buffer.UART_data.board_add, UART_in_buffer.UART_data.board_add, UART_in_buffer.UART_data.state, UART_in_buffer.UART_data.state);    
        }
        UDP_payload[current_index] = UART_in_buffer.UART_data;
        current_index++;
        memset(&UART_in_buffer, 0, sizeof(UART_in_buffer));
    }
}

/*##################################   Main   ################################*/

int main(){    

    /*##################################   INIT   ################################*/
    net.set_network(MASTER_IP,NETMASK,GATEWAY);
    net.connect();
    sock_out.open(&net);
    sock_in.open(&net);
    sock_in.bind(5005);

    /*##################################   INIT DONE   ################################*/
    queue.call_every(1, receive_led_UART);
    queue.call_every(1, receive_but_UART);
    queue.call_every(1, receive_oth_UART);  
    queue.call_every(100, receive_UDP); 
    queue.call_every(40, send_UDP);
    queue.dispatch_forever();
    
    while(true) {}
}
