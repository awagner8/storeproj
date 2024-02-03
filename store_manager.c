// Written by Alvin Lebeck (alvy@cs.duke.edu) 
// adapted from Porter Jones (pbjones@cs.washington.edu)
//
// This is a file for managing a store of various aisles,
// represented by an array of 64-bit integers

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_manager.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Priotizes and fills sections with lower indicies
 * first. Sections with lower indicies should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  for (int aisleIndex = 0; aisleIndex < NUM_AISLES; aisleIndex++) {
    for (int sectionIndex = 0; sectionIndex < SECTIONS_PER_AISLE; sectionIndex++) {
      while (stockroom[get_id(aisles[aisleIndex], sectionIndex)] > 0 && num_items(aisles[aisleIndex], sectionIndex) < 10) {
        aisles[aisleIndex] = add_items(aisles[aisleIndex], sectionIndex, 1);
        --stockroom[get_id(aisles[aisleIndex], sectionIndex)];
      }
    }
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short product_id, int quantity_requested) {
  if (quantity_requested <= 0) {
    return 0; 
  }

  int fulfilled_quantity = 0;

  for (int aisle = 0; aisle < NUM_AISLES; aisle++) {
    for (int section = 0; section < SECTIONS_PER_AISLE; section++) {
      if (get_id(aisles[aisle], section) == product_id) {
        while (num_items(aisles[aisle], section) > 0 && fulfilled_quantity < quantity_requested) {
          aisles[aisle] = remove_items(aisles[aisle], section, 1);
          fulfilled_quantity++;
          if (fulfilled_quantity == quantity_requested) {
            return quantity_requested;
          }
        }
      }
    }
  }

  int remaining_quantity = quantity_requested - fulfilled_quantity;
  if (stockroom[product_id] > 0 && remaining_quantity > 0) {
    if (stockroom[product_id] >= remaining_quantity) {
      stockroom[product_id] -= remaining_quantity;
      fulfilled_quantity += remaining_quantity;
    } else {
      fulfilled_quantity += stockroom[product_id];
      stockroom[product_id] = 0;
    }
  }

  return fulfilled_quantity; 
}

// Returns the aisle and section index of the first section with the given id 
// that has no items in it.
// Returns -1, -1 if no such section exists.
//
// Breaks ties by returning the section with the lowest aisle index then section index.
struct aisle_section empty_section_with_id(unsigned short id) {
//TODO: implement this function
  struct aisle_section result = {-1,-1};
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      if(get_id(aisles[i], j) == id && num_items(aisles[i], j) == 0) {
        struct aisle_section temp = {i, j};
        return temp;
      }
    }
  }
  return result;
}

// Returns the aisle and section index of the section with the highest inventory in the store.
//
// Breaks ties by returning the section with the lowest aisle index then section index.
struct aisle_section section_with_most_items() {
//TODO: implement this function
  //struct aisle_section result = {-1,-1};
  int aisle_index = 0;
  int section_index = 0;
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      if(num_items(aisles[aisle_index], section_index) < num_items(aisles[i], j)) {
        aisle_index = i;
        section_index = j;
        if(num_items(aisles[aisle_index], section_index) == 10) {
          struct aisle_section resultMax = {aisle_index, section_index};
          return resultMax;
        }
      }
    }
  }
  struct aisle_section result = {aisle_index, section_index};
  return result;
}
