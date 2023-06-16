/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btchiman <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 15:49:45 by btchiman          #+#    #+#             */
/*   Updated: 2023/06/12 15:49:50 by btchiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        void setMode(std::string mode);
        std::string getMode(void) const;
        void addClient(Client *client);
        std::vector<Client *> getClients(void) const;
        void addOperator(Client *client);
        std::vector<Client *> getOperators(void) const;
        void removeOperator(Client *client);
        void addBanned(int fd);
        void removeBanned(int fd);
        std::vector<int> getBanned(void) const;
        void addInvite(int fd);
        std::vector<int> getInvite(void) const;

    private:
        Channel(void);
        Channel(const Channel& other);
        Channel &operator=(const Channel &other);

        std::string _name;
        std::string _topic;
        std::string _password;
        std::string _mode;
        std::vector<Client *> _clients;
        std::vector<Client *> _operators;
        std::vector<int> _banned;
        std::vector<int> _invite;
        Client *_creator;
};


#endif
