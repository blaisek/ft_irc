# 🚀 Description

This project is about creating **_our own IRC server_**. We used an actual IRC client to connect to our server and test it.
**IRC** is a **protocol for real-time text messaging between internet-connected computers created in 1988**. It is mainly used for group discussion in chat rooms called “channels” although it supports private messages between two users, data transfer, and various server-side and client-side commands. As of April 2011, the top 100 IRC networks served over 500,000 users at a time on hundreds of thousands of channels." ([source](https://www.radware.com/security/ddos-knowledge-center/ddospedia/irc-internet-relay-chat/))

## Mandatory part

- We have to develop an IRC server in C++ 98.
- The server must be capable of handling multiple clients at the same time and never hang.
- Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
- An IRC client must be able to connect to your server without encountering any error.
- Communication between client and server has to be done via TCP/IP (v4).
- Using the IRC client with our server must be similar to using it with any official IRC server. However, we only have to implement the following features:
  - We must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using the IRC client.
  - All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
  - We must have operators and regular users.
  - Then, we have to implement the commands that are specific to operators.

## Bonus part

- Handle file transfer.
- Create a bot.

# 💻 Our IRC Commands

These are the available commands in our IRC Server:

- **NICK**: /nick nickname, Changes your nickname to a new nickname.
- **PASS**: /pass command is used to set a 'connection password'.
- **USER**: /user command is used at the beginning of connection to specify the username, hostname and realname of a new user.
- **PING**: /ping user, Sends a PING request to a user. Used for checking connection and lag. Servers issue pings on a timed basis to determine if users are still connected.
- **JOIN**: /join #channel [key] , Joins the specified channel with or without password.
- **MSG**: /msg nickname message, Sends a private message to nickname without opening a query window.
- **MODE**: /mode #channel|nickname [[+|-]modechars [parameters]], This is a powerful command that gives channel operators control of a channel and the users on it.
- **QUIT**: /quit [message], Disconnects you from IRC and will give the optional message as the reason for your departure. (this message only appears to people who are on the same channels as you).
- **PART**: /part command is used when you want to depart/leave a specific channel, or the current active channel. This command also allows you to specify a parting message, which will display to the remaining users a custom message in the server's PART message.
- **TOPIC**: /topic #channel newtopic, Changes the topic for a channel that you are on if you're an operator.
- **KICK**: /kick #channel nickname, Kicks a nickname off a channel that you are on if you're an operator.
- **INVITE**: /invite nickname #channel, Invites a nickname to a channel that you are on.
- **NOTICE**: /notice nickname message, Sends a private message to nickname without opening a query window for either you or them.
- **LIST**: /list command enables you to find list of channels on the server.
- **WHOIS**: /whois nickname, Shows you information about a nickname.
- **DIE**: /die command kill the server if the user is oper.
- **OPER**: /oper , gives root privilege to the user and requires the root password.

# ⚙️ Start IRC Server

To compile the program, use:

- `make`
  To start the Server, use:
- `./ircserv <port> <password>` - **port**: The port number on which your IRC server will be listening to for incoming IRC connections. - **password**: The connection password. It will be needed by any IRC client that tries to connect to your server.
  To connect to the server, you can use:
- `nc <IP ADDRESS> <PORT>`
  - **IP ADDRESS**: Host IP address.
  - **PORT**: The PORT that the server listening on.
- You can also use an IRC Client e.g irssi, WeeChat...

# 🧠 Contributors

[Simon Aeby](https://github.com/Laendrun)<br/>
&emsp;42intra profile : [saeby](https://profile.intra.42.fr/users/saeby) <br/>
<br/>
[Blaze Kalonda](https://github.com/blaisek)<br/>
&emsp;42intra profile : [btchiman](https://profile.intra.42.fr/users/btchiman) <br/>
<br/>

# 📚 Ressources

- [What is a Socket?](https://www.tutorialspoint.com/unix_sockets/what_is_socket.htm)
- [Unix Socket - Network Addresses](https://www.tutorialspoint.com/unix_sockets/network_addresses.htm)
- [Unix Socket - Core Functions](https://www.tutorialspoint.com/unix_sockets/socket_core_functions.htm)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
- [The UChicago χ-Projects](http://chi.cs.uchicago.edu/chirc/index.html)
- [Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [Internet Relay Chat: Architecture](https://datatracker.ietf.org/doc/html/rfc2810)
- [Internet Relay Chat: Channel Management](https://datatracker.ietf.org/doc/html/rfc2811)
- [Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Internet Relay Chat: Server Protocol](https://datatracker.ietf.org/doc/html/rfc2813)
- [IRSSI client](https://irssi.org/)
- [Basic IRC command](https://www.mirc.com/help/html/index.html?basic_irc_commands.html)

# 🔬 Schema

<img src="assets/irc-shema.png">
<br/>
<img src="assets/network.png">
