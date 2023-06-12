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

class Channel
{
    public:
        Channel(void);
        Channel(std::string name);
        Channel(const Channel& other);
        Channel &operator=(const Channel &other);
        ~Channel();

        std::string getName(void) const;
        void setName(std::string name);
        std::string getTopic(void) const;
        void setTopic(std::string topic);
        void addClient(Client *client);
        void removeClient(Client *client);
        void setPassword(std::string password);
        std::string getPassword(void) const;
        std::vector<Client *> getClients(void) const;

    private:
        std::string _name;
        std::string _topic;
        std::string _password;
        std::vector<Client *> _clients;
};


#endif
