# Message Protocol

## Message schema:

| **Field name:** | Message length | Message ID |  Token   | Payload | Check Sum |
|-----------------|:--------------:|:----------:|:--------:|:-------:|-----------|
| **Length:**     |    4 bytes     |   1 byte   | 40 bytes | ? bytes | 4 bytes   |

## Message IDs:

### User management:

1. register user
1. user registered confirmation *(from server)*
1. login user
1. user login confirmation *(from server)*
1. logout user
1. user logout confirmation *(from server)*

------------

### Friend mechanism:

7. send friend proposition
1. friend proposition send confirmation *(from server)*
1. new friend proposition info *(from server)*
1. friend proposition accept
1. friend proposition accept confirmation *(from server)*
1. new friend proposition accept info *(from server)*
1. friend proposition reject
1. friend proposition reject confirmation *(from server)*
1. new friend proposition reject info *(from server)*

------------

### Channel mechanism:

16. create channel
1. create channel confirmation *(from server)*
1. send invitation to channel
1. send invitation to channel confirmation *(from server)*
1. new invitation to channel info *(from server)*
1. invitation to channel accept
1. invitation to channel accept confirmation *(from server)*
1. new invitation to channel accept info *(from server)*
1. invitation to channel reject
1. invitation to channel reject confirmation *(from server)*
1. new invitation to channel reject info *(from server)*
1. leave channel
1. leave channel confirmation *(from server)*

------------

### Messages & Reactions:

29. send message
1. message send confirmation *(from server)*
1. check new message
1. new message response *(from server)*
1. new message info *(from server)*
1. request message(s)
1. message(s) response *(from server)*
1. isActive message
1. add reaction
1. reaction confirmation *(from server)*
1. request message reaction(s)
1. reaction(s) response *(from server)*
1. new reaction info *(from server)*
