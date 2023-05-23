# chatroom

### Setup local development:

1. ```sudo apt-get install build-essential```

2. ```sudo apt-get install qtbase5-dev```

3. ```sudo apt install librange-v3-dev```

4. Add qt to cmake prefix path ```-DCMAKE_PREFIX_PATH=/home/michal/Qt5/5.15.2/gcc_64```

### Run database:

```systemctl stop postgresql```

```docker-compose up```

### TODO:

* create user
* delete user
* update user
* create chatroom
* delete chatroom
* update chatroom
* join chatroom
* leave chatroom
* send message to chatroom
* receive message in chatroom
* direct messages between users
* add user to friends
* user has history of direct messages and messages in chatroom
* active users info
* message encryption
* show list of friends and chatrooms
* add admin user role in chatroom
* add reaction to messages
