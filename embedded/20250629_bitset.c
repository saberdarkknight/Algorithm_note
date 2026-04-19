/*
Question: Simple Bitset


Problem Description
Please implement a simple Bitset. 
This Bitset should be able to store up to N boolean values (N is specified by the user during creation). You need to implement the following functionalities:

Create Bitset:

A function Bitset* create_bitset(unsigned int size);

create_bitset(100); -> init all zero
set_bit(my_bitset, 99); -> set bit99 to true
test_bit(my_bitset, 1) -> false
test_bit(my_bitset, 99) -> true
set_bit(my_bitset, 101) -> return warning (return)

This function takes a size parameter, which represents the number of boolean values the Bitset can store.

It should dynamically allocate enough memory to store these bits and return a pointer to a Bitset structure.

Initially, all bits should be set to 0 (i.e., false).

If memory allocation fails, it should return NULL.

Set Bit:

A function void set_bit(Bitset* bs, unsigned int index);

This function sets the bit at the specified index to 1 (i.e., true).

You need to handle out-of-bounds index (e.g., by ignoring or printing an error message).

Clear Bit:

A function void clear_bit(Bitset* bs, unsigned int index);

This function sets the bit at the specified index to 0 (i.e., false).

Similarly, handle out-of-bounds index.

Test Bit:

A function bool test_bit(Bitset* bs, unsigned int index);

This function returns the boolean value of the bit at the specified index (true or false).

If index is out of bounds, it should return false or another reasonable default value.

Destroy Bitset:

A function void destroy_bitset(Bitset* bs);

This function frees all memory occupied by the Bitset.

int 4 bit
size = 6 => 2 int
set bit 5
int1  int0
0000  0000
0001  0000

*/


typedef struct {
    // implement here
	unsigned int* data; // used to store value
	int size;  
} Bitset;


// create bitset
Bitset* create_bitset(unsigned int size) {
	// example
     // user call size= 50
	// create xxx  -> 精度問題 
	// return structure
	Bitset* bitset_t = (Bitset *)malloc(sizeof(Bitset)) ;
	bitset_t->size = size;
	int num_ints = (size + (sizeof(unsigned int) * 8) - 1) / (sizeof(unsigned int) * 8);
	bitset_t->data = (unsigned int*)calloc(num_ints, sizeof(unsigned int));  // need to set data to 0
	// bitset_t->data = (int*)malloc(bitset_t->data, sizeof(int)*num_ints);
	return bitset_t;   // <------------
}

// set bit 50
void set_bit(Bitset* bs, unsigned int index) {
	// check if index out of bound
	if( index >= bs->size ){
		return;
	}

	// check position and set bit
	int pos = (index/(sizeof(int)*8));
	int remain = (index%(sizeof(int)*8));
	bs->data[pos] = bs->data[pos] | ( 1U << remain );

}

// clear bit
void clear_bit(Bitset* bs, unsigned int index) {

	// check if index out of bound
	if( index >= bs->size ){
		return;
	}

	// check position and set bit
	int pos = (index/(sizeof(int)*8));
	int remain = (index%(sizeof(int)*8));
	bs->data[pos] = bs->data[pos] & ~( 1U << remain );

}

// test bit
bool test_bit(Bitset* bs, unsigned int index) {

	// check if index out of bound
	if( index >= bs->size ){
		return false;
	}

	// check position and set bit
	int pos = (index/(sizeof(int)*8));
	int remain = (index%(sizeof(int)*8));
	// int bit = bs->data[pos] & ( 1U << remain );

	return ( bs->data[pos] & ( 1U << remain ) );
}

// destroy bitset
void destroy_bitset(Bitset* bs) {
	// check if bs is NULL
	if( !bs ){
		return;
	}
	bs->size = 0;
	free(bs->data);
	free(bs);

}


void flip_bit(Bitset* bs, unsigned int index) {
	// check if index out of bound
	if( index >= bs->size ){
		return;
	}

	// flip
		int pos = (index/(sizeof(int)*8));
	int remain = (index%(sizeof(int)*8));

	bs->data[pos] = bs->data[pos] ^ (1U << remain); 
}


/*
2 bitset AND operation

bitset1 bitset2 
set same size of each bitset
create a new bitset to store result = bitset1 and bitset2

return new bitset address
*/

Bitset* bitset_and(const Bitset* bs1, const Bitset* bs2) {
	//bs1 and bs2 are read only.

	Bitset* output = create_bitset(bs1->size);
	int pos = ( (bs1->size)/(sizeof(int)*8) );
	for( int i = 0; i < pos; i++ ){
		output->data[i] = bs1->data[i] & bs2->data[i];
	}
	int remaining_bits = bs1->size % (sizeof(unsigned int) * 8);
	if (remaining_bits > 0) {
        unsigned int mask = (1U << remaining_bits) - 1;
        result->data[pos] = (bs1->data[pos] & bs2->data[pos]) & mask;
    }

	return output;
}


