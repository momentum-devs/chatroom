# Message Protocol

v. 001

## Message schema:

| **Field name:** |  Message length | Message ID | Token | Payload | Check Sum |
|-----------------|:-------------:|:-------------:|:-------------:|:-------------:| ------------- |
| **Length:** | 4 bytes | 1 byte | 40 bytes | ? bytes | 4 bytes |

## Message IDs:

### User management:

{:start="1"}

1. register user
1. user registered confirmation *(from server)*
1. login user
1. user login confirmation *(from server)*
1. logout user
1. user logout confirmation *(from server)*

------------

### Friend mechanism:

{:start="7"}

1. send friend proposition
1. freind proposition send confirmation *(from server)*
1. new freind proposition info *(from server)*
1. friend proposition accept
1. freind proposition accept confirmation *(from server)*
1. new freind proposition accept info *(from server)*
1. friend proposition reject
1. friend proposition reject confirmation *(from server)*
1. new friend proposition reject info *(from server)*

------------

### Group mechanism:

{:start="16"}

1. create group
1. create group confirmation *(from server)*
1. send invitation to group
1. send invitation to group confirmation *(from server)*
1. new invitation to group info *(from server)*
1. invitation to group accept
1. invitation to group accept confirmation *(from server)*
1. new invitation to group accept info *(from server)*
1. invitation to group reject
1. invitation to group reject confirmation *(from server)*
1. new invitation to group reject info *(from server)*
1. leave group
1. leave group confirmation *(from server)*

------------

### Messages & Reactions:

{:start="29"}

1. send message
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