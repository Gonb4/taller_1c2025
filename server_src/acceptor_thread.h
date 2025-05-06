#ifndef ACCEPTOR_THREAD_H
#define ACCEPTOR_THREAD_H

#include <vector>
#include <algorithm>

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "client_thread.h"
#include "game_map.h"
#include "../common_src/liberror.h"

class AcceptorThread : public Thread {
    private:
        Socket skt;
        std::vector<ClientThread*> clients;
        GameMap& gm_ref;


        void reap_dead_clients() {
            clients.erase(
                std::remove_if(clients.begin(), clients.end(), 
                [](ClientThread* c) {
                    if (not c->is_alive()) {
                        c->join();
                        delete c;
                        return true;
                    }
                    return false;
                }), 
                clients.end()
            );
        }

        void kill_all_clients() {
            for (auto& th : clients) {
                if (th->is_alive())
                    th->force_stop();
            }
            for (auto& th : clients) {
                th->join();
                delete th;
            }
            clients.clear();
        }

    public:
        AcceptorThread(const std::string& port, GameMap& gm) : skt(port.c_str()), gm_ref(gm) {}

        void run() override {
            while (true) {
                try {
                    Socket peer_skt = skt.accept();
                    ClientThread* new_client = new ClientThread(std::move(peer_skt), gm_ref);
                    new_client->start();
                    clients.push_back(std::move(new_client));

                    reap_dead_clients();

                } catch (const LibError& err) { // cierre de server
                    kill_all_clients();
                }
            }
        }

            // crear objs Protocol y ClientThreads (startearlos)
            // reap de threads y partidas muertas?

            // while true:
            //     accept
            //     if caso skt cerrado ('q') LibError
            //         break
            //     ClientThread(Protocol(move(peer_skt))) // con new, y pushear en vector threads
            //     client_th->start()
            //     recorro vector threads: // esto lo puedo llamar reap_dead
            //         if not _is_alive
            //             join
            //             delete thread // hace falta join? yo diria que si
            //     game_map.wipe_empty_games()

            // // servidor cerro skt
            // recorro vector threads: // esto lo puedo llamar kill_threads o kill_clients // 2 TANDAS, SOCKETS 1RA JOINS 2DA
            //     if _is_alive:
            //         thread.kill_self cerrarle socket // lo puedo agregar en stop? guardo cada peer_skt - ClientThread? otro for antes de este que cierra todos los skt indiscriminadamente?
            //     thread.join                 
            //     delete thread
            // game_map.wipe_empty_games() // deberian estar todos vacios
        

        void force_stop() {
            skt.shutdown(2);
            skt.close();
        }

        ~AcceptorThread() {}

};

#endif
