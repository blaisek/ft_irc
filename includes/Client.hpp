
#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "irc.hpp"

class Client
{
	public:
		Client(void);
		Client(int fd);
		Client(const Client& other);
		Client &operator=(const Client &other);
		~Client();

		int							getFd(void) const;
		bool						isAuth(void) const;
        std::string                 getIp(void) const;
		std::string					getNick(void) const;
		std::string					getUser(void) const;
		std::string					getIdentity(void) const;
		std::string					getHost(void) const;
        std::string                 getChannelsNames(void);
		bool						getReg(void) const;
		std::string					getFullName(void) const;
		std::string					getModes(void) const;
        bool				        getMode(char mode) const;
		bool						isOp(void) const;
		std::vector<std::string>	getChans(void);
        time_t                      getIdleTime() const;
        std::string                 getSignOnTime() const;

        void                        setIdleTime();
        void                        setSignOnTime();
		void						setAuth(bool auth);
        void                        setIp(std::string ip);
		void						setNick(std::string nick);
		void						setUser(std::string user);
		void						setIdentity(std::string identity);
		void						setHost(std::string host);
		void						setReg(bool reg);
		void						setFullName(std::string fullname);
		void						setMode(char mode, bool setMode);
		void						join(std::string channelName);
		void						leave(std::string channelName);

	private:
		int							_fd;
		bool						_auth;
        std::string                  _ip;
		std::string					_nick;
		std::string					_user;
		std::string					_identity;
		std::string					_host;
		bool						_reg;
		std::string					_fullName;
		std::map<char, bool>		_modes;
		std::vector<std::string>	_channels;
        std::string                 _channelsNames;
        time_t                      _idleTime;
        std::string                 _signOnTime;
};

std::ostream	&operator<<(std::ostream const &o, Client &c);

#endif

