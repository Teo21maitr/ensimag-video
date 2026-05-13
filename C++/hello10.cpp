#include <condition_variable>
#include <cstdio>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

constexpr int NB_HELLO = 10;

struct Barriere {
    int count = 0;
    std::mutex mutex;
    std::condition_variable cond;
};

/* Appelé par chaque thread hello : incrémente le compteur et réveille
   les threads en attente quand le 10ième arrive. */
static void barriereArrivee(Barriere &b) {
    std::unique_lock<std::mutex> lock(b.mutex);
    b.count++;
    if (b.count == NB_HELLO)
        b.cond.notify_all();
}

/* Bloque jusqu'à ce que les NB_HELLO threads hello soient arrivés. */
static void barriereAttendre(Barriere &b) {
    std::unique_lock<std::mutex> lock(b.mutex);
    b.cond.wait(lock, [&b] { return b.count >= NB_HELLO; });
}

static void hello(Barriere *b) {
    printf("Hello World!\n");
    barriereArrivee(*b);
}

static void done(Barriere *b) {
    barriereAttendre(*b);
    printf("Done!\n");
}

int main() {
    auto b = std::make_unique<Barriere>();

    std::vector<std::thread> hello_threads;
    hello_threads.reserve(NB_HELLO);
    for (int i = 0; i < NB_HELLO; i++)
        hello_threads.emplace_back(hello, b.get());

    std::thread done_thread(done, b.get());

    for (auto &t : hello_threads)
        t.join();
    done_thread.join();

    return 0;
}
