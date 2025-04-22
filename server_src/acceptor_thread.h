#ifndef ACCEPTOR_THREAD_H
#define ACCEPTOR_THREAD_H

#include <vector>

#include "../common_src/thread.h"
#include "../common_src/socket.h"
// #include "../common_src/protocol.h"
// #include "client_thread.h"

class AcceptorThread : public Thread {
    private:
        Socket skt;
        std::vector<Thread*>& clients;
        // GameMap&

    public:
        void run() override { // caso socket cierra durante accept
            // crear objs Protocol y ClientThreads (startearlos)
        }

        ~AcceptorThread() {}

};

#endif
