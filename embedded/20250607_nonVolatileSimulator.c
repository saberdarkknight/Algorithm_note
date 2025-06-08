/*
Mock Interview Question: Flash Memory Simulation:

You are developing a Flash file system for a small embedded device. 
This system needs to be able to read/program/erase on Flash. 
To simplify the problem, let's assume that the Flash memory is divided into fixed-size "blocks," and each block can operate read/program/erase

Description: Please implement a C language module that simulates a simplified Flash storage interface, and then build upon it to implement a record write and read functionality.

Flash memory will be simulated by a byte array: 
uint8_t flash_memory[FLASH_SIZE].

FLASH_SIZE is a fixed value (4K Bytes) representing the total Flash memory size.

Flash memory is divided into fixed-size blocks, with each block having a size of BLOCK_SIZE. (256 Bytes)

All read/write operations must be byte-aligned.
Crucial: A write (program) operation can only change bits from 1 to 0. 
Any attempt to flip a bit from 0 to 1 (e.g., writing 0xFF to a location that is 0x00, or 0xAA to 0xBB) is not allowed, as this simulates the unidirectional bit-flipping characteristic of Flash memory. 
If an invalid write is attempted, your function should return an error.
An erase operation sets all bits within an entire block to 0xFF.

Function Prototype
Please implement the following functions:
void flash_init(void);
Initializes the simulated Flash memory, setting all bits to 0xFF.

int flash_read(uint32_t address, uint8_t *data, uint32_t size);
Reads size bytes from the given address into the data buffer.
Returns 0 on success, non-zero on failure.

int flash_program(uint32_t address, const uint8_t *data, uint32_t size);
Writes size bytes from the data buffer to the given address.
Must check for the validity of the write operation (only 1 to 0 bit flips are allowed).
Returns 0 on success, non-zero on failure.

int flash_erase_block(uint32_t block_address);
Erases the entire block starting from block_address. 
block_address must be the start address of a block.
Returns 0 on success, non-zero on failure.

You can use error code and macro as follows
// macro
#define FLASH_SIZE      (4 * 1024)  // 4 KB Flash
#define BLOCK_SIZE      (256)       // 256 B per block
#define RECORD_SIZE     (32)        // 32 B per record (31 B data + 1 B CRC)
// memory virtualization
// uint8_t flash_memory[FLASH_SIZE]

#define ERR_OK                      0
#define ERR_INVALID_ADDRESS         -1
#define ERR_INVALID_SIZE            -2
#define ERR_INVALID_WRITE_OPERATION -3
#define ERR_NO_SPACE                -4
#define ERR_CRC_MISMATCH            -5
#define ERR_ALIGNMENT_ERROR         -6

*/



#define FLASH_SIZE      (4 * 1024)  // 4 KB Flash
#define BLOCK_SIZE      (256)       // 256 B per block
#define RECORD_SIZE     (32)        // 32 B per record (31 B data + 1 B CRC)

#define ERR_OK                       0
#define ERR_INVALID_ADDRESS         -1
#define ERR_INVALID_SIZE            -2
#define ERR_INVALID_WRITE_OPERATION -3
#define ERR_NO_SPACE                -4
#define ERR_CRC_MISMATCH            -5
#define ERR_ALIGNMENT_ERROR         -6


// flash array
static char flash [FLASH_SIZE];

// initial the memory array
void flash_init(void){
	memset( flash, 0xFF, FLASH_SIZE );
}


int flash_read(uint32_t address, uint8_t *data, uint32_t size){
	// check if address over limit
	if( address >= FLASH_SIZE){
		return ERR_INVALID_ADDRESS;
	}

	// check if address + size will over single block
	if ( address%BLOCK_SIZE + size >= BLOCK_SIZE ){
		return ERR_INVALID_SIZE;
	}	

	// use memset to allocate data
	memcpy(data, &flash[address], size); 
	return ERR_OK;
}

// program unit : no limit => remenber byte alignment
// 0xAA 1010  0xBB 1011  
// 0xAA 0xAA 0xAA 0xAA 0xAA
// bit from 0 to 1 is not allowed
// 1->1 : OK  0->0 : OK  1->0 : OK  0->1 : Err
// 6 : 0110  5 : 0101  ~6 = 1001  ~6&5=0001
int flash_program(uint32_t address, const uint8_t *data, uint32_t size){
	// check if address over limit
	if ( address >= FLASH_SIZE ){
		return ERR_INVALID_ADDRESS;
	}

	// check alignment
	if ( address%BLOCK_SIZE + size >= BLOCK_SIZE ){
		return ERR_INVALID_SIZE;
	}

	// program the memory 
	// check if there is bit flipping
	for ( uint32_t i = 0; i < size; i++ ){
		uint32_t cur = flash[address + i];
		if ( (~cur) & data[i] ){
			return ERR_INVALID_WRITE_OPERATION;
		} else {
			flash[address + i] = data[i]; 
			// flash[address + i] &= data[i]; // it is beeter not to use assign
		}
	}

	return ERR_OK;
}




// erase unit : block
int flash_erase_block(uint32_t block_address){
	// check if block_address over limit
	if ( block_address >= FLASH_SIZE ){
		return ERR_INVALID_ADDRESS;
	}

	// check whether block_address is alignment
	// block_address = 100 => 0-255
	// block_address = 255 => 0-255
	// block_address = 256 => 256-511
	uint32_t start = (block_address/256)*256;
	memset( &flash[start], 0xFF, BLOCK_SIZE);

	return ERR_OK;
}

