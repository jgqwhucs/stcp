

#pragma once

#include <stcp/arch/dpdk/rte.h>
#include <queue>
#include <mutex>
#include <stdio.h>
#include <stdarg.h>
#include <stcp/ncurses.h>
#include <stcp/filefd.h>


namespace stcp {





using eth_conf = struct rte_eth_conf;
using mbuf = struct rte_mbuf;
using mempool = struct rte_mempool;
using ip_frag_death_row = struct rte_ip_frag_death_row;
using ip_frag_tbl       = struct rte_ip_frag_tbl;



class pkt_queue {
    std::queue<mbuf*> queue;
public:
    void push(mbuf* msg)
    {
        queue.push(msg);
    }
    mbuf* pop()
    {
        mbuf* msg = queue.front();
        queue.pop();
        return msg;
    }
    size_t size() const
    {
        return queue.size();
    }
    bool empty() const
    {
        return queue.empty();
    }
};


template<class T>
class queue_TS {
    std::queue<T> queue;
    mutable std::mutex m;
    using auto_lock=std::lock_guard<std::mutex>;
public:
    void push(T msg)
    {
        auto_lock lg(m);
        queue.push(msg);
    }
    T pop()
    {
        auto_lock lg(m);
        T msg = queue.front();
        queue.pop();
        return msg;
    }
    size_t size() const
    {
        auto_lock lg(m);
        return queue.size();
    }
    bool empty() const
    {
        auto_lock lg(m);
        return queue.empty();
    }
};

} /* namespace */
