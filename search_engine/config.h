#ifndef _CONFIG_H
#define _CONFIG_H

#include <cstdint>
#include <queue>
#include <unordered_map>

#include "ring.h"
#include "task_info.h"
#include "constants.h"


struct GlobalVars 
{
    /* --------------------------------
     * --variables for search process -
     * --------------------------------*/
    const char *  dfs_host;
    uint16_t      dfs_port;
    const char *  dfs_root;
    const char *  storage_backend;
    uint16_t      dfs_file_reader_thread_num;
    uint16_t      dfs_file_reader_batch_size;
    uint16_t      enable_total_matches;

    /* --------------------------------
     * --variables for daemon process -
     * --------------------------------*/

    /* Time interval (minutes) when task shall become old and
     * be removed from the `task_pool`*/
    uint16_t      task_stale_interval;

    /* PayloadBlock ring */
    Ring<PB_RING_SIZE>  pb_r;

    /* FileList ring */
    Ring<FL_RING_SIZE>  fl_r;

    /* Task information pool */
    std::unordered_map<std::string, TaskInfo> task_pool;

    /* Task running pool */
    std::unordered_map<std::string, pid_t> running_pool;

    /* Task pending queue */
    std::queue<std::string> pending_queue;

    /* log level */
    const char *  log_level;

    /* monitor thread's tid */
    pid_t monitor_tid;
};

class Config 
{
    public:
        /*
         * Name:
         *  Config::instance
         *
         * Description:
         *  This function is called when caller wanna a Config instance.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  Config instance.
         *
         * NOTE:
         *  None.
         */
        static Config &instance() {
            static Config s_config;
            return s_config;
        }

        /*
         * Name:
         *  Config::init
         *
         * Description:
         *  None.
         *
         * Parameters:
         *  conf_file - [IN] configuration file.
         *
         * Returns:
         *  true, success.
         *  false, failure.
         *
         * NOTE:
         *  None.
         */
        bool init(const char *conf_file);

        /*
         * Name:
         *  Config::reload
         *
         * Description:
         *  This function is used to reload configuration file.
         *
         * Parameters:
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  None.
         */
        void reload();

        /*
         * Name:
         *  Config::dump_configs
         *
         * Description:
         *  This function is used to display configurations.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  None.
         */
        void dump_configs();

        GlobalVars *get_global_vars() { return m_ptr_gvars; }

    private:
        Config();
        ~Config();

        void init_from_config_file();

        GlobalVars *m_ptr_gvars;
        char *m_conf_file;
};

/*
 * Name:
 *  GetConfig
 *
 * Description:
 *  This function is used to get the Config instance.
 *
 * Parameters:
 *  None.
 * 
 * Return:
 *  pointer which points at a Config instance.
 *
 * NOTE:
 *  None.
 */
inline Config &GetConfig() {
    return Config::instance();
}

/*
 * Name:
 *  GetGlobalVars
 *
 * Description:
 *  This function is used to get the GlobalVars instance.
 *
 * Parameters:
 *  None.
 * 
 * Return:
 *  pointer which points at a GlobalVars instance.
 *
 * NOTE:
 *  None.
 */
inline GlobalVars *GetGlobalVars() {
    return GetConfig().get_global_vars();
}

/*
 * Name:
 *  payload_block_ring_push
 *  file_list_ring_push
 *
 * Description:
 *  This function is simply a wrapper function for Ring::push
 *  For more details, please refer the related implementation.
 * 
 * Parameters:
 *  obj        - [IN] element to be put (PayloadBlock instance).
 *  wait_timed - [IN] whether invoke condition and time wait.
 *  time_out   - [IN] seconds for time out. (only useful when wait_timed is true).
 * 
 * Returns:
 *  None.
 *
 * NOTE:
 *  None.
 */
inline void payload_block_ring_push(void *obj, 
                                    bool wait_timed=true, 
                                    unsigned int time_out=3) {
    GetGlobalVars()->pb_r.push(obj, wait_timed, time_out);
}

inline void file_list_ring_push(void *obj, 
                                bool wait_timed=true, 
                                unsigned int time_out=3) {
    GetGlobalVars()->fl_r.push(obj, wait_timed, time_out);
}

/*
 * Name:
 *  payload_block_ring_pop
 *  file_list_ring_pop
 *
 * Description:
 *  This function is simply a wrapper function for Ring::pop.
 *  For more details, please refer the related implementation.
 * 
 * Parameters:
 *  wait_timed - [IN] whether invoke condition and time wait.
 *  time_out   - [IN] seconds for time out. (only useful when wait_timed is true).
 * 
 * Returns:
 *  Element dequeued from ring.
 *
 * NOTE:
 *  None.
 */
inline void *payload_block_ring_pop(bool wait_timed=true, 
                                    unsigned int time_out=3) {
    return GetGlobalVars()->pb_r.pop(wait_timed, time_out);
}

inline void *file_list_ring_pop(bool wait_timed=true, 
                                unsigned int time_out=3) {
    return GetGlobalVars()->fl_r.pop(wait_timed, time_out);
}

#endif
