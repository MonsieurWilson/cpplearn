
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "configs.h"
#include "log.h"

using namespace rapidjson;


GlobalVars::GlobalVars() : m_storage_backend("hdfs"),
                           m_zmq_mode("PULL"),
                           m_dfs_host("localhost"),
                           m_dfs_root("snappy"),
                           m_dfs_loglevel("ERROR"),
                           m_dfs_port(9000), 
                           m_zmq_port_num(0),
                           m_timer_span(4),
                           m_flush_packet_group_interval(10),
                           m_flush_parquet_file_interval(30),
                           m_enable_compression(0),
                           m_enable_spark(0),
                           m_enable_second_level_dir(0),
                           m_log_level("INFO")
{
}

GlobalVars::~GlobalVars()
{
}


Configs::Configs() : m_ptrVars(nullptr)
{
}

Configs::~Configs()
{
    if( m_ptrVars ) {
        delete m_ptrVars;
    }
    for (size_t idx = 0; idx < PACKET_GROUP_WORK_RING_COUNT; ++idx) {
        if( nullptr != packet_group_work_ring[idx] ) {
            xdf::destory_rw_ring(packet_group_work_ring[idx]);
        }
    }
    for (size_t idx = 0; idx < PARQUET_MEM_WORK_RING_COUNT; ++idx) {
        if( nullptr != parquet_mem_work_ring[idx] ) {
            xdf::destory_rw_ring(parquet_mem_work_ring[idx]);
        }
    }
    if( nullptr != packet_group_resource_ring ) {
        delete packet_group_resource_ring;
    }
    if( nullptr != parquet_mem_resource_ring ) {
        delete parquet_mem_resource_ring;
    }
}

void Configs::show_all_ring_stat() 
{
    for (size_t idx = 0; idx < PACKET_GROUP_WORK_RING_COUNT; ++idx) {
        LOG_DEBUG("packet_group_work_ring-{}: {}, parquet_mem_work_ring-{}: {}",
                  idx, xdf::rw_ring_entries(packet_group_work_ring[idx]), 
                  idx, xdf::rw_ring_entries(parquet_mem_work_ring[idx]));
    }
}

void Configs::init_from_config_file(const char *conf_file)
{
    FILE *fp = fopen(conf_file, "rb");

    if (nullptr == fp) {
        LOG_WARN("failed to open configuration file, "
                 "the program will use default values.");
        return;
    }

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    rewind(fp);

    char *json_contents = (char *)malloc(sizeof(char) * file_size);
    FileReadStream fs(fp, json_contents, file_size);
    Document doc;
    doc.ParseStream(fs);
    fclose(fp);

    auto get_string_val_by_key = [&doc](const char *key, 
                                        const char *default_val=nullptr) {
        return doc.HasMember(key) ? doc[key].GetString() : default_val;
    };

    auto get_int_val_by_key    = [&doc](const char *key, 
                                        const int default_val=0) {
        return doc.HasMember(key) ? doc[key].GetInt() : default_val;
    };

    m_ptrVars->m_storage_backend = 
                             get_string_val_by_key("storage_backend", "hdfs");
    m_ptrVars->m_dfs_host = get_string_val_by_key("dfs_host", "localhost");
    m_ptrVars->m_dfs_port = get_int_val_by_key("dfs_port", 9000);
    m_ptrVars->m_dfs_root = get_string_val_by_key("dfs_root", "/snappy");
    m_ptrVars->m_enable_compression =
                             get_int_val_by_key("enable_compression", 0);
    m_ptrVars->m_enable_spark =
                             get_int_val_by_key("enable_spark", 0);
    m_ptrVars->m_enable_second_level_dir =
                             get_int_val_by_key("enable_second_level_dir", 0);
    m_ptrVars->m_flush_parquet_file_interval =
                             get_int_val_by_key("parquet_flush_interval", 30);
    m_ptrVars->m_flush_packet_group_interval =
                             get_int_val_by_key("packet_flush_interval", 10);
    m_ptrVars->m_log_level = get_string_val_by_key("log_level", "INFO");

    free(json_contents);

    dump_configs();
}

bool Configs::init(const char *conf_file)
{
    m_ptrVars = new GlobalVars();
    if( nullptr == m_ptrVars ) {
        LOG_CRITICAL("create GlobalVars error!");
        return false;
    }

    /* Load values according to configuration file */
    init_from_config_file(conf_file);

    /* Initalize ring buffer */
    for (size_t idx = 0; idx < PACKET_GROUP_WORK_RING_COUNT; ++idx) {
        packet_group_work_ring[idx] = xdf::create_rw_ring(WORK_RING_CACHE_COUNT);
        if( nullptr == packet_group_work_ring[idx] ) {
            LOG_CRITICAL("create packet_group_work_ring-{} error!", idx);
            return false;
        }
    }

    for (size_t idx = 0; idx < PARQUET_MEM_WORK_RING_COUNT; ++idx) {
        parquet_mem_work_ring[idx] = xdf::create_rw_ring(PARQUET_FILE_CHUNK_COUNT);
        if( nullptr == parquet_mem_work_ring[idx] ) {
            LOG_CRITICAL("create parquet_mem_work_ring-{} error!", idx);
            return false;
        }
    }

    if( nullptr == (packet_group_resource_ring = new (std::nothrow) xdf::pg_res_ring)) {
        LOG_CRITICAL("create packet_group_resource_ring error!");
        return false;
    }

    if( nullptr == (parquet_mem_resource_ring = new (std::nothrow) xdf::pq_res_ring)) {
        LOG_CRITICAL("create parquet_mem_resource_ring error!");
        return false;
    }

    uint8_t* bucket;
    PacketGroup* packet_group;
    for( size_t i = 0; i < PACKET_GROUP_COUNT; ++i ) {
        bucket = (uint8_t*)malloc(sizeof(PacketGroup) + MAX_PACKET_COUNT * MAX_PACKET_LENGTH);
        if( nullptr == bucket ) {
            LOG_CRITICAL("malloc packet_group error!");
            return false;
        }
        packet_group = (PacketGroup*)bucket;
        bucket += sizeof(PacketGroup);
        for( size_t j = 0; j < MAX_PACKET_COUNT; ++j ) {
            packet_group->payload_values[j].ptr = bucket;
            bucket += MAX_PACKET_LENGTH;
        }
        put_packet_group_resource(packet_group);
    }

    for( size_t i = 0; i < PARQUET_MEM_COUNT; ++i ) {
        bucket = (uint8_t*)malloc(MAX_PARQUET_FILE_SIZE);
        if( nullptr == bucket ) {
            LOG_CRITICAL("malloc parquet_mem error!");
            return false;
        }
        put_parquet_mem_resource(bucket);
    }

    return true;
}

void Configs::reload(const char *conf_file)
{
    LOG_INFO("reload from configuration file.");
    init_from_config_file(conf_file);
    GetLog().set_log_level(m_ptrVars->m_log_level);
}

void Configs::dump_configs()
{
    LOG_DEBUG("load \"storage_backend\" from configuration file: {}", 
              m_ptrVars->m_storage_backend);
    LOG_DEBUG("load \"dfs_host\" from configuration file: {}", 
              m_ptrVars->m_dfs_host);
    LOG_DEBUG("load \"dfs_port\" from configuration file: {}", 
              m_ptrVars->m_dfs_port);
    LOG_DEBUG("load \"dfs_root\" from configuration file: {}", 
              m_ptrVars->m_dfs_root);
    LOG_DEBUG("load \"enable_compression\" from configuration file: {}", 
              m_ptrVars->m_enable_compression);
    LOG_DEBUG("load \"enable_spark\" from configuration file: {}", 
              m_ptrVars->m_enable_spark);
    LOG_DEBUG("load \"enable_second_level_dir\" from configuration file: {}", 
              m_ptrVars->m_enable_second_level_dir);
    LOG_DEBUG("load \"log_level\" from configuration file: {}", 
              m_ptrVars->m_log_level);

}
