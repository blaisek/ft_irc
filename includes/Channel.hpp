
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Client;

class Channel
{
    public:

        Channel(std::string name);
        ~Channel();

        std::string getName(void) const;
        void setName(std::string name);
        std::string getTopic(void) const;
        void setTopic(std::string topic);
        void removeClient(Client *client);
        void setPassword(std::string password);
        std::string getPassword(void) const;
        bool hasPassword(void) const;
        void addMode(std::string mode);
        void removeMode(std::string mode);
        std::string			getModes(void) const;
        bool				getMode(char mode) const;
        void addClient(Client *client);
        std::vector<Client *> getClients(void) const;
        void addOperator(Client *client);
        std::vector<Client *> getOperators(void) const;
        void removeOperator(Client *client);
        std::map<std::string, std::string> getBanned(void) const;
		void				removeUser(std::string nick);
		void				setMode(char mode, bool setMode);
		void				setLimit(int limit);
		int					getLimit(void) const;
		void				addInvited(std::string nick);
		void				removeInvited(std::string nick);
		std::vector<std::string>	getInvited(void) const;
		bool				isInvited(std::string nick) const;
		bool				isOp(std::string nick) const;
		Client*				getClient(std::string nick) const;
        bool hasNickname(std::string nickname) const;
        bool find(int fd);
        bool empty(void) const;
        std::string getNicknamesList(void) const;
        void removeNickname(std::string nickname);
        void addNickname(std::string nickname);
		std::vector<std::string>	getNickNames(void) const;

		void				ban(std::string nick, std::string bannedBy);
		void				unban(std::string nick);
		bool				isBanned(std::string nick);
		int					getUserCount(void) const;

    private:
        Channel(void);
        Channel(const Channel& other);
        Channel &operator=(const Channel &other);

        std::string						_name;
        std::string						_topic;
        std::string						_password;
		std::vector<Client *>			_clients;
		std::vector<Client *>			_operators;
		std::vector<std::string>		_invited;
		std::map<std::string, std::string>	_bannedNick;
        std::vector<int>				_banned;
        std::vector<std::string>		_nicknames;
		std::map<char, bool>			_modes;
		int								_limit;
};


#endif
