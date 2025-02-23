// main.cpp
#include "../mailsender.h/mailsender.h"
#include <iostream>

int main() {
    mailsender sender("ace116d02f391e", "fabde6d4102347", "sandbox.smtp.mailtrap.io", 2525);

    if (sender.sendEmail("from@example.com", "to@example.com", "Test Email", "Hello, this is a test email using mailsender class!")) {
        std::cout << "Email sent successfully!" << std::endl;
    }
    else {
        std::cerr << "Failed to send email." << std::endl;
    }

    return 0;
}
