// mailsender.h
#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <string>

class mailsender {
public:
    mailsender(const std::string& username, const std::string& password, const std::string& smtpServer, int port);
    bool sendEmail(const std::string& from, const std::string& to, const std::string& subject, const std::string& body);

private:
    struct upload_status {
        const char* payload;
        size_t bytesRead;
    };

    static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp);

    std::string username;
    std::string password;
    std::string smtpServer;
    int port;
};

#endif
#pragma once
