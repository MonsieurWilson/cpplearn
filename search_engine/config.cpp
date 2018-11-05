#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "log.h"
#include "config.h"

using namespace rapidjson;


/* ------------Config--------------*/
Config::Config() : m_ptr_gvars(nullptr), m_conf_file(nullptr) {}

Config::~Config() {
    if (m_ptr_gvars) {
        delete m_ptr_gvars;
    }
    if (m_conf_file) {
        free(m_conf_file);
    }
}

bool Config::init(const char *conf_file) {
    m_ptr_gvars = new GlobalVars();
    if (nullptr == m_ptr_gvars) {
        return false;
    }

    m_conf_file = (char *)malloc(strlen(conf_file)+1);
    if (nullptr == m_conf_file) {
        return false;
    }
    strcpy(m_conf_file, conf_file);

    /* Initialize default values */
    m_ptr_gvars->dfs_host                   = "localhost";
    m_ptr_gvars->dfs_port                   = 9000;
    m_ptr_gvars->dfs_root                   = "/snappy";
    m_ptr_gvars->storage_backend            = "hdfs";
    m_ptr_gvars->dfs_file_reader_thread_num = 1;
    m_ptr_gvars->dfs_file_reader_batch_size = 64;
    m_ptr_gvars->enable_total_matches       = 0;
    m_ptr_gvars->log_level                  = "INFO";
    m_ptr_gvars->task_stale_interval        = 10;

    /* Load values according to configuration file */
    init_from_config_file();

    return true;
}

void Config::reload() {
    init_from_config_file();
}

void Config::dump_configs() {
    LOG_DEBUG("Load \"dfs_host\" from configuration file: {}", 
              m_ptr_gvars->dfs_host);
    LOG_DEBUG("Load \"dfs_port\" from configuration file: {}", 
              m_ptr_gvars->dfs_port);
    LOG_DEBUG("Load \"dfs_root\" from configuration file: {}", 
              m_ptr_gvars->dfs_root);
    LOG_DEBUG("Load \"storage_backend\" from configuration file: {}", 
              m_ptr_gvars->storage_backend);
    LOG_DEBUG("Load \"reader_thread_num\" from configuration file: {}", 
              m_ptr_gvars->dfs_file_reader_thread_num);
    LOG_DEBUG("Load \"reader_batch_size\" from configuration file: {}", 
              m_ptr_gvars->dfs_file_reader_batch_size);
    LOG_DEBUG("Load \"enable_total_matches\" from configuration file: {}", 
              m_ptr_gvars->enable_total_matches);
    LOG_DEBUG("Load \"log_level\" from configuration file: {}", 
              m_ptr_gvars->log_level);
    LOG_DEBUG("Load \"task_stale_interval\" from configuration file: {}", 
              m_ptr_gvars->task_stale_interval);
}

void Config::init_from_config_file() {
    FILE *fp = fopen(m_conf_file, "rb");
    if (nullptr == fp) {
        LOG_WARN("Failed to open configuration file,"
                 "the program will use default values");
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

    /* Load configuration values from json document */
    if (doc.HasMember("dfs_host")) {
        m_ptr_gvars->dfs_host = doc["dfs_host"].GetString();
    }
    if (doc.HasMember("dfs_port")) {
        m_ptr_gvars->dfs_port = doc["dfs_port"].GetInt();
    }
    if (doc.HasMember("dfs_root")) {
        m_ptr_gvars->dfs_root = doc["dfs_root"].GetString();
    }
    if (doc.HasMember("storage_backend")) {
        m_ptr_gvars->storage_backend = 
            doc["storage_backend"].GetString();
    }
    if (doc.HasMember("dfs_file_reader_thread_num")) {
        m_ptr_gvars->dfs_file_reader_thread_num = 
            doc["dfs_file_reader_thread_num"].GetInt();
    }
    if (doc.HasMember("dfs_file_reader_batch_size")) {
        m_ptr_gvars->dfs_file_reader_batch_size =
            doc["dfs_file_reader_batch_size"].GetInt();
    }
    if (doc.HasMember("enable_total_matches")) {
        m_ptr_gvars->enable_total_matches =
            doc["enable_total_matches"].GetInt();
    }
    if (doc.HasMember("log_level")) {
        m_ptr_gvars->log_level = doc["log_level"].GetString();
    }
    if (doc.HasMember("task_stale_interval")) {
        m_ptr_gvars->task_stale_interval = 
            doc["task_stale_interval"].GetInt();
    }

    free(json_contents);

    GetLog().set_log_level(m_ptr_gvars->log_level);
}

