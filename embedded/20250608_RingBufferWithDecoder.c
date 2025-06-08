/*
Question

In embedded systems, it's common to receive data from serial interfaces (e.g., UART, SPI, I2C). 
This data is often transmitted in packages, each with a specific format, such as a sync byte, data length, actual data, a checksum, and an end byte. To efficiently process this incoming data without loss, we typically use a Ring Buffer.
Your task is to implement a feature that reads raw byte streams from a ring buffer and decodes complete data packages.

--Package Format--
Assume our data packages follow this simple format:

|Field	   |Size (bytes)  |         Description
|SYNC_BYTE |      1       |Package header identifier, fixed value 0xAA                                    |
|LENGTH	   |      1       |Length of the data content (N), excluding SYNC_BYTE, LENGTH, CHECKSUM, END_BYTE|
|DATA	   |      N       |Actual data content                                                            |
|CHECKSUM  |	  1       |	Simple sum (modulo 256) of all bytes from LENGTH to the end of DATA           |
|END_BYTE  |      1	      |Package footer identifier, fixed value 0x55                                    |

Example Package:
0xAA 0x03 0x01 0x02 0x03 0x09 0x55

Where:
* SYNC_BYTE: 0xAA
* LENGTH: 0x03 (indicates 3 bytes of data follow)
* DATA: 0x01 0x02 0x03
* CHECKSUM: (0x03 + 0x01 + 0x02 + 0x03) % 256 = 0x09
* END_BYTE: 0x55

--Requirements--
I. Implement a Ring Buffer

1. You should define data structure by yourself
suggested data structure
typedef struct {

} RingBuffer;

2. It should include at least the following APIs:
ring_buffer_init(RingBuffer *rb, uint8_t *buffer, size_t capacity): Initializes the ring buffer.
ring_buffer_write(RingBuffer *rb, uint8_t byte): Writes a single byte to the ring buffer. State your chosen behavior if the buffer is full (e.g., return error or overwrite oldest data).
ring_buffer_read(RingBuffer *rb, uint8_t *byte): Reads a single byte from the ring buffer. Returns an error if the buffer is empty.
ring_buffer_peek(RingBuffer *rb, size_t offset, uint8_t *byte): Peeks at a byte at a specific offset in the ring buffer without removing it.
ring_buffer_bytes_available(RingBuffer *rb): Returns the current number of bytes available for reading in the buffer.
ring_buffer_advance_read_ptr(RingBuffer *rb, size_t count): Advances the read pointer by a specified number of bytes (useful for removing a full package after decoding).


II. Follow-up: Implement a Package Decoder

In your main function, you need to:
1. Initialize a ring buffer.
2. Simulate receiving a byte stream containing complete and/or incomplete packages (e.g., use a static array as input, writing chunks to the ring buffer).
3. Repeatedly call your package decoder until all data in the ring buffer has been processed.
4. Print the content of each successfully parsed package and report any parsing errors.
*/

/* Function Prototypes */

#define cSYNC_BYTE               0xAA
#define cEND_BYTE                0x55
#define cMAX_PACKAGE_DATA_LENGTH 250 // Assume max data length within a package
#define cMIN_PACKAGE_SIZE        254

/*
ring buffer structure 
|SYNC_BYTE |      1       
|LENGTH    |      1       
|DATA      |      N       
|CHECKSUM  |      1      
|END_BYTE  |      1 


| buff[0] | buff[1] | buffer[2] ... 
 SYNC_BYTE  LENGTH

*/

typedef struct {
    uint8_t head;
    uint8_t tail;
    char* data;
    int capacity;
    int count;
} RingBuffer;


typedef struct {
    uint8_t data[cMAX_PACKAGE_DATA_LENGTH];
    uint8_t length; // Actual data length
} Package;

typedef enum {
    DECODER_STATE_WAIT_SYNC,
    DECODER_STATE_GET_LENGTH,
    DECODER_STATE_GET_DATA,
    DECODER_STATE_GET_CHECKSUM,
    DECODER_STATE_GET_END_BYTE
} DecoderState;

typedef struct {
    DecoderState state;
    Package current_package;     // Used to buffer the package being parsed
    uint8_t current_length;      // Expected data length for current package
    uint8_t received_data_count; // How many data bytes have been received so far
    uint8_t calculated_checksum; // Checksum for the current package
} PackageDecoder;


enum {
    STATUS_SUCCESS = 0,
    STATUS_INIT_ERROR = -1,
    STATUS_RINGBUFFER_FULL = -2,
    STATUS_RINGBUFFER_EMPTY = -3,
    STATUS_PEEK_OVERRANGE = -4,
    STATUS_INVALID_SIZE = -5,
    STATUS_NULL_PTR = -6,
    // please remove or do not use it if you do not need it.
};




// initial ring buffer
int ring_buffer_init(RingBuffer *rb, uint8_t *buffer, size_t capacity){
    // check capacity
    if( capacity < cMIN_PACKAGE_SIZE ){
        return STATUS_INVALID_SIZE;
    }

    // check if NULL input
    if( !rb || !buffer ){
        return STATUS_NULL_PTR;
    }

    // initialize the buffer
    rb->head = 0;
    rb->tail = 0;
    rb->capacity = capacity;
    rb->count = 0;
    rb->data = buffer;

    return STATUS_SUCCESS;
}

int ring_buffer_write(RingBuffer *rb, uint8_t byte){
    // check if NULL input (cehck rb->data instead of byte)
    if( !rb || !rb->data ){
        return STATUS_NULL_PTR;
    }

    // check if buffer is full
    if( rb->count >= rb->capacity ){
        return STATUS_RINGBUFFER_FULL;
    }

    // start write the buffer
    rb->data[rb->tail] = byte;
    rb->count ++;
    rb->tail = (rb->tail +1)%rb->capacity;;

    return STATUS_SUCCESS;
}

int ring_buffer_read(RingBuffer *rb, uint8_t *byte){
    // check if NULL input
    if( !rb || !rb->data ){
        return STATUS_NULL_PTR;
    }

    // check if buffer is empty
    if( rb->count == 0 ){
        return STATUS_RINGBUFFER_EMPTY;
    }

    // start write the buffer
    *byte = rb->data[rb->head]; // remember to assign value instead of address or use byte[0]
    rb->count--;
    rb->head = ( rb->head - 1 + rb->capacity)%rb->capacity;
    return STATUS_SUCCESS;
}

int ring_buffer_peek(RingBuffer *rb, size_t offset, uint8_t *byte){
    // check if NULL input
    if( !rb || !byte ){
        return STATUS_NULL_PTR;
    }

    // check offset
    if ( (offset > rb->capacity) || (offset < 0) ){
        return STATUS_INVALID_SIZE;
    }

    // check if offset is over count
    if ( rb->head + offset > rb->tail ){
        return STATUS_PEEK_OVERRANGE;
    }

    // peek the value
    *byte = rb->data[(rb->head + offset)%rb->capacity];
    return STATUS_SUCCESS;
}

// check the available number of reading
size_t ring_buffer_bytes_available(RingBuffer *rb){
    // check if NULL pointer
    if( !rb || !rb->data ){
        return 0;  // return 0 instead of error code
    }

    size_t num = rb->count;
    return num;
}

int ring_buffer_advance_read_ptr(RingBuffer *rb, size_t count){
    // check if NULL pointer
    if( !rb ){
        return STATUS_NULL_PTR;
    }

    // check count 
    if( count > rb->count ){
        return STATUS_INVALID_SIZE;
    }

    // update head
    rb->head = (rb->head + count)%rb->capacity;

    return STATUS_SUCCESS;
}



// stop here
void package_decoder_init(PackageDecoder *decoder){



}

int package_decoder_process(PackageDecoder *decoder, RingBuffer *rb, Package *out_package){
    
    
}
