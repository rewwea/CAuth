#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "UserManager.h"
#include "Logger.h"

using namespace std;
using boost::asio::ip::tcp;

class NetworkManager {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    UserManager& userManager;

public:
    NetworkManager(UserManager& um, short port = 1234)
        : acceptor(io_context, tcp::endpoint(tcp::v4(), port)), userManager(um) {}

    void start() {
        Logger::log("Server started on port 1234");
        acceptConnections();
        io_context.run();
    }

private:
    void acceptConnections() {
        acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                Logger::log("Client connected");
                handleClient(move(socket));
            }
            acceptConnections();
        });
    }

    void handleClient(tcp::socket socket) {
        auto buf = make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(socket, *buf, '\n',
            [this, buf, sock = move(socket)](boost::system::error_code ec, size_t) mutable {
                if (!ec) {
                    istream is(buf.get());
                    string line;
                    getline(is, line);
                    Logger::log("Received: " + line);
                    string response = processCommand(line);
                    response += "\n";
                    boost::asio::async_write(sock, boost::asio::buffer(response),
                        [sock = move(sock)](boost::system::error_code, size_t) {});
                }
            });
    }

    string processCommand(const string& cmd) {
        if (cmd.find("REGISTER:") == 0) {
            auto parts = split(cmd, ':');
            if (parts.size() != 4) return "REGISTER_FAILED";
            bool ok = userManager.registerUser(parts[1], parts[2], parts[3]);
            return ok ? "REGISTER_SUCCESS" : "REGISTER_FAILED";
        } else if (cmd.find("LOGIN:") == 0) {
            auto parts = split(cmd, ':');
            if (parts.size() != 3) return "AUTH_FAILED";
            bool ok = userManager.auth(parts[1], parts[2]);
            return ok ? "AUTH_SUCCESS" : "AUTH_FAILED";
        } else if (cmd.find("CHECK_ACCESS:") == 0) {
            auto parts = split(cmd, ':');
            if (parts.size() != 3) return "ACCESS_DENIED";
            bool ok = userManager.hasAccess(parts[1], parts[2]);
            return ok ? "ACCESS_GRANTED" : "ACCESS_DENIED";
        }
        return "INVALID_COMMAND";
    }

    vector<string> split(const string& s, char delim) {
        vector<string> parts;
        string item;
        for (char ch : s) {
            if (ch == delim) {
                parts.push_back(item);
                item.clear();
            } else {
                item += ch;
            }
        }
        if (!item.empty()) parts.push_back(item);
        return parts;
    }
};