#include "task_info.h"


TaskInfo::TaskInfo() {
    const char    *empty_str_val = "";
    const uint64_t empty_int_val = 0;
    Pointer("/mode").Set(m_doc, empty_str_val);
    Pointer("/uuid").Set(m_doc, empty_str_val);
    Pointer("/ctime").Set(m_doc, empty_str_val);
    Pointer("/status").Set(m_doc, empty_str_val);
    Pointer("/stime").Set(m_doc, empty_str_val);
    Pointer("/etime").Set(m_doc, empty_str_val);
    Pointer("/file_name").Set(m_doc, empty_str_val);
    Pointer("/file_size").Set(m_doc, empty_int_val);
    Pointer("/sip").Set(m_doc, empty_int_val);
    Pointer("/dip").Set(m_doc, empty_int_val);
    Pointer("/sport").Set(m_doc, empty_int_val);
    Pointer("/dport").Set(m_doc, empty_int_val);
    Pointer("/proto").Set(m_doc, empty_int_val);
    Pointer("/limit").Set(m_doc, empty_int_val);
}

TaskInfo::~TaskInfo() {}

TaskInfo::TaskInfo(TaskInfo &&t) {
    m_doc = std::move(t.m_doc);
}

TaskInfo &TaskInfo::operator=(TaskInfo &&t) {
    m_doc = std::move(t.m_doc);
    return *this;
}

bool TaskInfo::load(const char *js) {
    ParseResult res = m_doc.Parse(js);
    return !res.IsError();
}

bool TaskInfo::load(const std::string &js) {
    return load(js.c_str());
}

const char *TaskInfo::json_string() {
    m_buffer.Clear();
    Writer<StringBuffer> writer(m_buffer);
    m_doc.Accept(writer);
    return m_buffer.GetString();
}

const char *TaskInfo::get_str_val_by_key(const char *key) {
    return m_doc[key].GetString();
}

void TaskInfo::set_str_val_by_key(const char *key, const char *val) {
    m_doc[key].SetString(val, strlen(val), m_doc.GetAllocator());
}

uint64_t TaskInfo::get_int_val_by_key(const char *key) {
    return m_doc[key].GetUint64();
}

void TaskInfo::set_int_val_by_key(const char *key, uint64_t val) {
    m_doc[key].SetUint64(val);
}

