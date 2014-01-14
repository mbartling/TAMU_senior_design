 
#include <stdio.h>
#include <stdlib.h>
#include "my-debug.h"

/* may receive wrong value if ptr is not 2-byte aligned */
//uint16_t value = *(uint16_t*)ptr;
/* portable way of reading a little-endian value */
//uint16_t value = *(uint8_t*)ptr
//                | ((*(uint8_t*)(ptr+1))<<8);

#define MAX_ENTRIES 32

const uint8_t SF = 0x7E;

struct entry_t
{
	struct entry_t * next;
	void 	* data;
	int32_t   data_len;
};

struct entry_t default  = { NULL,  NULL, 0};

const struct entry_t end_entry = {NULL, NULL, 0};


struct fast_entry_t
{
	void 	* data;
	int16_t   data_len;
};

struct fast_packet_t
{
	struct fast_entry_t entry[MAX_ENTRIES];
	int					  num_entries;
};
void create_sf(struct fast_entry_t * entry)
{
	entry->data 	= &SF;
	entry->data_len = 1;
}

int write_data( struct fast_entry_t *payload,
				void 				*in_data,
				int16_t 			data_len)
{
	payload->data = in_data;
	payload->data_len = data_len;
	return data_len;
}

/* \brief form_tx
 *
 */

//int form_tx_packet( struct fast_entry *packet, 
//					/* Output */ int  *num_entries,

int send_tx_packet(fast_packet_t *packet)
{
	int i, j;
	uint8_t sum;
	int num_bytes;
	fast_entry_t current_entry;

	for (i = 0; i < packet->num_entries; i++)
	{
		current_entry = packet->entry[i];
		num_bytes += current_entry.data_len;
		for(j = 0; j < current_entry.data_len; j++)
		{
			putc(current_entry.data[j], STDOUT);
		}
	}

}
