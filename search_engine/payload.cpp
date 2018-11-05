#include "payload.h"
#include "log.h"


/* ---------------Buffer---------------*/
Buffer::Buffer(uint8_t *ptr, const uint64_t len)
    : m_ptr(ptr), m_len(len) {}

Buffer::~Buffer() {
    if (m_ptr) {
        free(m_ptr);
        m_ptr = nullptr;
    }
}

/* ----------------Payload--------------- */
Payload::Payload(const uint64_t ts, const uint64_t len, uint8_t *ptr) 
    : m_ts(ts) {
   if (len && ptr) {
       m_buf = std::make_shared<Buffer>(ptr, len);
   } else {
       m_buf = nullptr;
   }
}

Payload::Payload(const Payload &p) {
    set_time_stamp(p.get_time_stamp());
    m_buf = p.get_buffer();
}

/* ---------------PayloadBlock---------------- */
PayloadBlock::PayloadBlock(Payload *p, int f)
    : payload(p), flag(f) {}

