// mailsender.cpp
#include "mailsender.h"
#include <curl/curl.h>
#include <iostream>
#include <cstring>

mailsender::mailsender(const std::string& username, const std::string& password, const std::string& smtpServer, int port)
    : username(username), password(password), smtpServer(smtpServer), port(port) {
}

size_t mailsender::payload_source(void* ptr, size_t size, size_t nmemb, void* userp) {
    struct upload_status* uploadCtx = (struct upload_status*)userp;
    size_t buffer_size = size * nmemb;

    if (uploadCtx->payload[uploadCtx->bytesRead]) {
        size_t len = strlen(uploadCtx->payload + uploadCtx->bytesRead);
        len = (len > buffer_size) ? buffer_size : len;
        memcpy(ptr, uploadCtx->payload + uploadCtx->bytesRead, len);
        uploadCtx->bytesRead += len;
        return len;
    }
    return 0;
}

bool mailsender::sendEmail(const std::string& from, const std::string& to, const std::string& subject, const std::string& body) {
    CURL* curl;
    CURLcode res = CURLE_OK;

    std::string payload = "From: " + from + "\r\n";
    payload += "To: " + to + "\r\n";
    payload += "Subject: " + subject + "\r\n\r\n";
    payload += body + "\r\n";

    struct upload_status uploadCtx = { payload.c_str(), 0 };

    curl = curl_easy_init();
    if (!curl) return false;

    std::string url = "smtp://" + smtpServer + ":" + std::to_string(port);

    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &uploadCtx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    res = curl_easy_perform(curl);

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}
