//configs.h

#ifndef _XDF_CONFIGS_H
#define _XDF_CONFIGS_H

#include <stdint.h>
#include <boost/asio.hpp>
#include <vector>

#include "packet_define.h"
#include "rw_ring.h"
#include "mrmw_ring.h"


class GlobalVars
{
public:
    std::string              m_storage_backend;
    std::string              m_zmq_mode;
    std::vector<std::string> m_zmq_url;
    std::string              m_dfs_host;
    std::string              m_dfs_root;
    std::string              m_dfs_loglevel;
    uint16_t                 m_dfs_port;
    int                      m_zmq_port_num;
    int                      m_timer_span;
    int                      m_flush_packet_group_interval;
    int                      m_flush_parquet_file_interval;
    int                      m_enable_compression;
    int                      m_enable_spark;
    int                      m_enable_second_level_dir;
    std::string              m_log_level;

public:
    GlobalVars();
    ~GlobalVars();
};

class Configs
{
public:
    GlobalVars *              m_ptrVars;
    boost::asio::io_service   io;
    xdf::rw_ring *            packet_group_work_ring[PACKET_GROUP_WORK_RING_COUNT];
    xdf::rw_ring *            parquet_mem_work_ring[PARQUET_MEM_WORK_RING_COUNT];
    xdf::pg_res_ring *        packet_group_resource_ring;
    xdf::pq_res_ring *        parquet_mem_resource_ring;

public:
    bool init(const char *conf_file);
    void reload(const char *conf_file);
    void dump_configs();
    void show_all_ring_stat(void);

    inline PacketGroup *     get_packet_group_resource(void);
    inline bool              put_packet_group_resource(PacketGroup* pg);
    inline PacketGroup *     read_packet_group_work(size_t idx);
    inline bool              write_packet_group_work(PacketGroup* pg);
    inline uint8_t *         get_parquet_mem_resource(void);
    inline bool              put_parquet_mem_resource(uint8_t* bucket);
    inline ParquetMemChunk * read_parquet_mem_work(size_t idx);
    inline bool              write_parquet_mem_work(ParquetMemChunk* buffer, size_t idx);

    static Configs &instance(void)
    {
        // function-local static to force this to work correctly at static initialization time.
        static Configs s_configs;
        return s_configs;
    }

private:
    Configs();
    ~Configs();
    void init_from_config_file(const char *conf_file);

};

PacketGroup* Configs::get_packet_group_resource(void)
{
    return (PacketGroup *)xdf::mrmw_ring_sc_dequeue(packet_group_resource_ring);
}

bool Configs::put_packet_group_resource(PacketGroup* pg)
{
    return xdf::mrmw_ring_sp_enqueue(packet_group_resource_ring, pg);
}

PacketGroup* Configs::read_packet_group_work(size_t idx)
{
    return (PacketGroup*)xdf::rw_ring_sc_dequeue(packet_group_work_ring[idx]);
}

bool Configs::write_packet_group_work(PacketGroup* pg)
{
    static size_t idx = 0;
    bool ret = false;
    size_t end = idx + PACKET_GROUP_WORK_RING_COUNT;
    for (; idx != end; ++idx) {
        size_t ring_idx = idx % PACKET_GROUP_WORK_RING_COUNT;
        if (xdf::rw_ring_sp_enqueue(packet_group_work_ring[ring_idx], pg)) {
            xdf::rw_ring_notify_one(packet_group_work_ring[ring_idx]);
            ret = true;
            break;
        }
    }
    ++idx;
    return ret;
}

uint8_t* Configs::get_parquet_mem_resource(void)
{
    return (uint8_t *)xdf::mrmw_ring_sc_dequeue(parquet_mem_resource_ring);
}

bool Configs::put_parquet_mem_resource(uint8_t* bucket)
{
    return xdf::mrmw_ring_sp_enqueue(parquet_mem_resource_ring, bucket);
}

ParquetMemChunk* Configs::read_parquet_mem_work(size_t idx)
{
    return (ParquetMemChunk*)xdf::rw_ring_sc_dequeue(parquet_mem_work_ring[idx]);
}

bool Configs::write_parquet_mem_work(ParquetMemChunk* buffer, size_t idx)
{
    return xdf::rw_ring_sp_enqueue(parquet_mem_work_ring[idx], buffer);
}

static inline Configs &GetConfigs(void)
{
    return Configs::instance();
}

static inline GlobalVars* GetGlobalVars(void)
{
    return GetConfigs().m_ptrVars;
}

#endif //_CONFIGS_H
