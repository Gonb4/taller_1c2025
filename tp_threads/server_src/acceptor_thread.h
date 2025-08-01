#ifndef ACCEPTOR_THREAD_H
#define ACCEPTOR_THREAD_H

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "../common_src/liberror.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "client_thread.h"
#include "game_map.h"

class AcceptorThread: public Thread {
private:
    Socket skt;
    std::vector<ClientThread*> clients;
    GameMap& gm_ref;


    void reap_dead_clients() {
        clients.erase(std::remove_if(clients.begin(), clients.end(),
                                     [](ClientThread* c) {
                                         if (not c->is_alive()) {
                                             c->join();
                                             delete c;
                                             return true;
                                         }
                                         return false;
                                     }),
                      clients.end());
    }

    void kill_all_clients() {
        for (auto& th: clients) {
            if (th->is_alive())
                th->force_stop();
        }
        for (auto& th: clients) {
            th->join();
            delete th;
        }
        clients.clear();
    }


public:
    AcceptorThread(const std::string& port, GameMap& gm): skt(port.c_str()), gm_ref(gm) {}

    void run() override {
        try {
            while (true) {
                Socket peer_skt = skt.accept();
                ClientThread* new_client = new ClientThread(std::move(peer_skt), gm_ref);
                new_client->start();
                clients.push_back(std::move(new_client));

                reap_dead_clients();
            }
        } catch (const LibError& err) {  // cierre de server
            kill_all_clients();
        }
    }

    void force_stop() {
        skt.shutdown(2);
        skt.close();
    }

    ~AcceptorThread() {}
};

#endif
