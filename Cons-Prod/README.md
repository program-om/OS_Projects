## Description

This program is two main functions producer and consumer. The producer creates an item and place it in the buffer and the consumer get the item and remove it from the buffer. There are to two helper functions which are insert_item and remove item. The producer uses insert_item to place a new item in the buffer and the consumer uses remove_item to remove an item from the buffer. The program used two semaphore variables to ensure that the producer waits when the buffer is full until one of the item is consumed and the consumer waits when the buffer is empty until an item is produced then the consumer could consume it.

