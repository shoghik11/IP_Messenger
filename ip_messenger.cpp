#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct Contact {
    std::string name;
    std::string ip;
    int port;
};

std::vector<Contact> contacts;
std::mutex contacts_mutex;
int listen_port;

void save_contacts() {
    std::ofstream f("contacts.txt");
    for (auto &c : contacts) {
        f << c.name << " " << c.ip << " " << c.port << "\n";
    }
}

void load_contacts() {
    std::ifstream f("contacts.txt");
    if (!f) return;
    contacts.clear();
    Contact c;
    while (f >> c.name >> c.ip >> c.port) {
        contacts.push_back(c);
    }
}

void print_contacts() {
    std::lock_guard<std::mutex> lock(contacts_mutex);
    std::cout << "--- Contacts ---\n";
    int i = 1;
    for (auto &c : contacts) {
        std::cout << i++ << ". " << c.name << " - " << c.ip << ":" << c.port << "\n";
    }
}

void receive_messages() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) { perror("socket"); return; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(listen_port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return; }
    if (listen(server_fd, 5) < 0) { perror("listen"); return; }

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);
        if (client < 0) continue;

        char buffer[1024];
        int n = read(client, buffer, sizeof(buffer)-1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "\n[New Message]: " << buffer << "\n> ";
            fflush(stdout);
        }
        close(client);
    }
}

void send_message(const std::string &ip, int port, const std::string &from, const std::string &to_msg) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    std::string message = "<message to=\"" + ip + "\" from=\"" + from + "\" type=\"chat\" xmlns=\"jabber:client\">"
                          "<body>" + to_msg + "</body></message>";

    send(sock, message.c_str(), message.size(), 0);
    close(sock);
}

int main() {
    std::cout << "Enter your listening port: ";
    std::cin >> listen_port;
    std::cin.ignore();

    load_contacts();

    std::thread receiver(receive_messages);
    receiver.detach();

    std::string user_name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, user_name);

    while (true) {
        std::cout << "\n--- IP Messenger ---\n"
                  << "1. Add contact\n"
                  << "2. List contacts\n"
                  << "3. Remove contact\n"
                  << "4. Edit contact\n"
                  << "5. Send message\n"
                  << "6. Exit\n> ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            Contact c;
            std::cout << "Name: "; std::getline(std::cin, c.name);
            std::cout << "IP: "; std::getline(std::cin, c.ip);
            std::cout << "Port: "; std::cin >> c.port; std::cin.ignore();
            std::lock_guard<std::mutex> lock(contacts_mutex);
            contacts.push_back(c);
            save_contacts();
        } else if (choice == 2) {
            print_contacts();
        } else if (choice == 3) {
            print_contacts();
            int idx; std::cin >> idx; std::cin.ignore();
            std::lock_guard<std::mutex> lock(contacts_mutex);
            if (idx > 0 && idx <= contacts.size()) contacts.erase(contacts.begin()+idx-1);
            save_contacts();
        } else if (choice == 4) {
            print_contacts();
            int idx; std::cin >> idx; std::cin.ignore();
            if (idx > 0 && idx <= contacts.size()) {
                std::cout << "New Name: "; std::getline(std::cin, contacts[idx-1].name);
                std::cout << "New IP: "; std::getline(std::cin, contacts[idx-1].ip);
                std::cout << "New Port: "; std::cin >> contacts[idx-1].port; std::cin.ignore();
                save_contacts();
            }
        } else if (choice == 5) {
            print_contacts();
            int idx; std::cin >> idx; std::cin.ignore();
            if (idx > 0 && idx <= contacts.size()) {
                std::string msg;
                std::cout << "Message: "; std::getline(std::cin, msg);
                send_message(contacts[idx-1].ip, contacts[idx-1].port, user_name, msg);
            }
        } else if (choice == 6) {
            break;
        }
    }

    return 0;
}
