#include <vector>
#include <tuple>

#include <string>   // string
#include <chrono>   // timer
#include <iostream> // cout, endl
#include <mutex>
#include <vector>
#include <windows.h>

#include "lib/utils.h"
#include "lib/model.h" 
#include "lib/embedding.h" 
#include "lib/instruction.h"


namespace proj1 {

void run_one_instruction(Instruction inst, EmbeddingHolder* users, EmbeddingHolder* items, std::mutex* mtx1, std::mutex* mtx2, std::vector<unsigned> lock1, std::vector<unsigned> lock2, std::mutex* mtx) {
    switch (inst.order) {
        case INIT_EMB: {
            // We need to init the embedding
            int length = users->get_emb_length();
            Embedding* new_user = new Embedding(length);
            mtx->lock();
            int user_idx = users->append(new_user);
            mtx->unlock();
            mtx1->lock();
            for (int item_index : inst.payloads) {
                Embedding* item_emb = items->get_embedding(item_index);
                // Call cold start for downstream applications, slow
                EmbeddingGradient* gradient = cold_start(new_user, item_emb);
                users->update_embedding(user_idx, gradient, 0.01);
                delete gradient;
            }
            mtx1->unlock();
            break;
        }
        case UPDATE_EMB: {
            int user_idx = inst.payloads[0];
            int item_idx = inst.payloads[1];
            int label = inst.payloads[2];
            // You might need to add this state in other questions.
            // Here we just show you this as an example
            // int epoch = -1;
            //if (inst.payloads.size() > 3) {
            //    epoch = inst.payloads[3];
            //}
            bool flag = 0;
            do {
                mtx2->lock();
                for (int i = 0; i < lock1.size(); i++) {
                    if (user_idx == lock1[i]) {
                        flag = 1;
                    }
                }
                for (int i = 0; i < lock2.size(); i++) {
                    if (item_idx == lock2[i]) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    lock2.push_back(item_idx);
                    lock1.push_back(user_idx);
                } 
                mtx2->unlock();
                if (flag == 1) {
                    Sleep(1);
                }
            } while (flag == 1);
            Embedding* user = users->get_embedding(user_idx);
            Embedding* item = items->get_embedding(item_idx);
            EmbeddingGradient* gradient = calc_gradient(user, item, label);
            users->update_embedding(user_idx, gradient, 0.01);
            delete gradient;
            gradient = calc_gradient(item, user, label);
            items->update_embedding(item_idx, gradient, 0.001);
            delete gradient;
            mtx2->lock();
            std::remove(lock1.begin(),lock1.end(),user_idx);
            std::remove(lock2.begin(),lock2.end(),item_idx);
            mtx2->unlock();

            break;
        }
        case RECOMMEND: {
            int user_idx = inst.payloads[0];
            Embedding* user = users->get_embedding(user_idx);
            std::vector<Embedding*> item_pool;
            int iter_idx = inst.payloads[1];
            for (unsigned int i = 2; i < inst.payloads.size(); ++i) {
                int item_idx = inst.payloads[i];
                item_pool.push_back(items->get_embedding(item_idx));
            }
            Embedding* recommendation = recommend(user, item_pool);
            recommendation->write_to_stdout();
            break;
        }
    }

}
} // namespace proj1

int main(int argc, char* argv[]) {

    std::vector<unsigned> lock1;
    std::vector<unsigned> lock2;
    std::mutex m; 
    std::mutex m1; 
    std::mutex m2; 
    std::mutex* mtx1 = &m1;
    std::mutex* mtx2 = &m2;
    std::mutex* mtx = &m;
    proj1::EmbeddingHolder* users = new proj1::EmbeddingHolder("data/q0.in");
    proj1::EmbeddingHolder* items = new proj1::EmbeddingHolder("data/q0.in");
    proj1::Instructions instructions = proj1::read_instructrions("data/q0_instruction.tsv");
    {
        proj1::AutoTimer timer("q0");  // using this to print out timing of the block
        // Run all the instructions
        for (proj1::Instruction inst : instructions) {
            proj1::run_one_instruction(inst, users, items, mtx1, mtx2, lock1, lock2, mtx);
        }
    }

    // Write the result
    users->write_to_stdout();
    items->write_to_stdout();

    // We only need to delete the embedding holders, as the pointers are all
    // pointing at the emb_matx of the holders.
    delete users;
    delete items;

    return 0;
}