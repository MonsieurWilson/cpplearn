#ifndef _TASK_INFO_H
#define _TASK_INFO_H

#include <string>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/writer.h>

using namespace rapidjson;


class TaskInfo 
{
    public:
        /*
         * Schema:
         * {
         *     @ string value
         *     "mode"      - client mode
         *
         *     "uuid"      - UUID
         *
         *     "ctime"     - task creation time
         *
         *     "status"    - task status
         *
         *     "stime"     - start time [search options]
         *
         *     "etime"     - end time [search options]
         *
         *     "file_name" - file path
         *
         *     "file_size" - file size
         *
         *     @ integer value
         *     "sip"       - source IP address [search options]
         *
         *     "dip"       - destination IP address [search options]
         *
         *     "sport"     - source port number [search options]
         *
         *     "dport"     - destination port number [search options]
         *
         *     "proto"     - IP proto number [search options]
         *
         *     "limit"     - threshold of maximum matched packet size
         * }
         */
        TaskInfo();

        ~TaskInfo();

        /* Name:
         *  TaskInfo::TaskInfo
         * 
         * Description:
         *  `TaskInfo` Move constructor function.
         *
         * Parameters:
         *  t - rvalue reference of another `TaskInfo` object.
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  None.
         */
        TaskInfo(TaskInfo &&t);

        /* Name:
         *  TaskInfo::operator=
         * 
         * Description:
         *  Overloading operator =.
         *
         * Parameters:
         *  t - rvalue reference of another `TaskInfo` object.
         *
         * Returns:
         *  refencence of `TaskInfo` object.
         *
         * NOTE:
         *  None.
         */
        TaskInfo &operator=(TaskInfo &&t);

        /*
         * Name:
         *  TaskInfo::load
         * 
         * Description:
         *  This function is used to parse json strings into
         *  DOM.
         *
         * Parameters:
         *  js - [IN] json format string (C String or C++ basic_string
         *
         * Returns:
         *  true, success.
         *  false, failure.
         *
         * NOTE:
         *  None.
         */
        bool load(const char *js);

        bool load(const std::string &js);

        /*
         * Name:
         *  TaskInfo::json_string
         *
         * Description:
         *  This function is used to translate DOM into json
         *  string.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  a json format string.
         *
         * NOTE:
         *  None.
         */
        const char *json_string();

        /*
         * Name:
         *  TaskInfo::get_str_val_by_key
         *
         * Description:
         *  Getter function for string values.
         *
         * Parameters:
         *  key - [IN] name of the DOM node
         *
         * Returns:
         *  the related value of the DOM node
         *
         * NOTE:
         *  None.
         */
        const char *get_str_val_by_key(const char *key);

        /*
         * Name:
         *  TaskInfo::set_str_val_by_key
         *
         * Description:
         *  Setter function for string values.
         *
         * Parameters:
         *  key - [IN] name of the DOM node
         *  val - [IN] value of the DOM node
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  None.
         */
        void set_str_val_by_key(const char *key, const char *val);

        /*
         * Name:
         *  TaskInfo::get_int_val_by_key
         *
         * Description:
         *  Getter function for integer values.
         *
         * Parameters:
         *  key - [IN] name of the DOM node
         *
         * Returns:
         *  the related value of the DOM node
         *
         * NOTE:
         *  None.
         */
        uint64_t get_int_val_by_key(const char *key);

        /*
         * Name:
         *  TaskInfo::set_int_val_by_key
         *
         * Description:
         *  Setter function for integer values.
         *
         * Parameters:
         *  key - [IN] name of the DOM node
         *  val - [IN] value of the DOM node
         *
         * Returns:
         *  None.
         *
         * NOTE:
         *  None.
         */
        void set_int_val_by_key(const char *key, uint64_t val);

        TaskInfo(const TaskInfo &t) = delete;

        TaskInfo &operator=(const TaskInfo &t) = delete;

    private:
        Document             m_doc;
        StringBuffer         m_buffer;
};

#endif
