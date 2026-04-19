/*
Normally, Node is a structure with two fields: data and a pointer to the next node (Node(String value, Node next)). 
In this problem, we use a SecuredLinkedList data structure where Node has an additional field: int hashValue 
(Node(String value, Node next, int hashValue)).

The hashValue of a Node should be the hash of "the Node's own value concatenated with the hashValue of the next Node". 
If the Node does not have a next Node, its hashValue should be the hash of only the Node's own value.

You can assume there is an already existing hash function: int calcHash(String input). 
The hash function implementation is not important for this question.

The SecuredLinkedList data structure helps to ensure the data integrity and prevent memory-based attacks. 
We want you to define and implement a SecuredLinkedList class which has the following two interfaces:

void addValue(String value) which adds a new node with the value at the "head" of the linked list. 
Note: This is different from a normal Linked List which usually adds a new node to the tail.

boolean isValidChain() which returns true if the whole chain is valid, false if not. "Valid" 
means for every node, the hashValue is in compliance with the hash calculation as defined above.


Follow-up 


1. we assume you are a hacker who wants to hack the chain by inserting a new value into the list.
please implement the hack function with the following interface
void hack(String value, int pos, SecuredLinkedList chain)

2. How can we check if there is a cycle in the list?

3. Given a normal linklist, how can we change the list to secured linklist?
*/