
#include "irc.hpp"

std::string Server::_cmd_topic(Request& req, int fd)
{

    std::string topic = req.trailing;
    std::string channel = req.params[0];
    std::string nick = this->_clients[fd]->getNick();
    std::string msg;

    // check if channel exist and if user is on it
    if(this->_channels.find(channel) == this->_channels.end())
        return (this->_get_message(nick, ERR_NOSUCHCHANNEL, "No such channel"));
    if(!this->_channels[channel]->hasNickname(nick))
        return (this->_get_message(nick, ERR_NOTONCHANNEL, "You're not on that channel"));

    // check if topic not empty, if channel mode t and if user is not op
    if(!topic.empty() && this->_channels[channel]->getMode('t') && !this->_channels[channel]->isOp(nick))
        return (this->_get_message(nick, ERR_CHANOPRIVSNEEDED, "You're not channel operator"));

    // check if topic not empty, if channel mode t and if user is op
    else if(!topic.empty() && this->_channels[channel]->getMode('t') && this->_channels[channel]->isOp(nick))
    {
        this->_channels[channel]->setTopic(topic);
        msg = ":" + nick + " TOPIC " + channel + " :" + topic + "\r\n";
        this->sendMessageToChannelUsers(channel,msg,fd);
        return msg;
    }
    // check if topic not empty and if channel not mode t
    else if(!topic.empty() && !this->_channels[channel]->getMode('t'))
    {
        this->_channels[channel]->setTopic(topic);
        msg = ":" + nick + " TOPIC " + channel + " :" + topic + "\r\n";
        this->sendMessageToChannelUsers(channel,msg,fd);
        return msg;
    }

    else if(topic.empty() && this->_channels[channel]->getTopic().empty())
        return (this->_get_message(nick, RPL_NOTOPIC, "No topic is set"));
    else if(topic.empty() && !this->_channels[channel]->getTopic().empty())
        return (this->_get_message(nick, RPL_TOPIC, this->_channels[channel]->getTopic()));
    else
        return (this->_get_message(nick, ERR_UNKNOWNERROR, "Unknown error"));
}
