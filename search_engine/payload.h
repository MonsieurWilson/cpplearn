#ifndef _PAYLOAD_H
#define _PAYLOAD_H

#include <memory>


class Buffer
{
    public:
        Buffer(uint8_t *ptr, const uint64_t len);

        ~Buffer();

        /*
         * Name:
         *  Buffer::get_ptr
         *
         * Description:
         *  This function is used to get the pointer of stored data.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  the inner pointer which stores in Buffer.
         *
         * NOTE:
         *  None.
         */
        uint8_t *get_ptr() const { return m_ptr; }

        /*
         * Name:
         *  Buffer::get_len
         *
         * Description:
         *  This function is used to get the length of stored data.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  the length of buffer.
         *
         * NOTE:
         *  None.
         */
        uint64_t get_len() const { return m_len; }

        Buffer(const Buffer &b) = delete;

        Buffer &operator=(const Buffer &b) = delete;

    private:
        /* the pointer which points at byte data */
        uint8_t *m_ptr;
        /* the length of byte data */
        uint64_t m_len;
};

class Payload
{
    public:
        Payload(const uint64_t ts  =0, 
                const uint64_t len =0,
                uint8_t *ptr =nullptr);

        Payload(const Payload &p);

        ~Payload() {}

        /*
         * Name:
         *  get_time_stamp
         *
         * Description:
         *  This function is used to get the timestamp of stored payload.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  time stamp
         *
         * NOTE:
         *  None.
         */
        uint64_t get_time_stamp() const { return m_ts; }

        /*
         * Name:
         *  get_payload_len
         *
         * Description:
         *  This function is used to get the length of stored payload.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  the length of inner buffer.
         *
         * NOTE:
         *  None.
         */
        uint64_t get_payload_len() const { return m_buf->get_len(); }

        /*
         * Name:
         *  get_payload_ptr
         *
         * Description:
         *  This function is used to get the pointer of stored payload.
         *
         * Parameters:
         *  None.
         *
         * Returns:
         *  pointer which points at the inner byte array.
         *
         * NOTE:
         *  None.
         */
        uint8_t *get_payload_ptr() const { return m_buf->get_ptr(); }

        std::shared_ptr<Buffer> get_buffer() const { return m_buf; }

        void set_time_stamp(const uint64_t ts) { m_ts = ts; }

        void set_payload(uint8_t *ptr, const int64_t len) {
            if (len && ptr) {
                m_buf = std::make_shared<Buffer>(ptr, len);
            }
        }

    private:
        /* time stamp */
        uint64_t m_ts;
        /* pointer which points at Buffer */
        std::shared_ptr<Buffer> m_buf;
};

struct PayloadBlock 
{
    std::shared_ptr<Payload>  payload;
    int                       flag;

    PayloadBlock(Payload *p=nullptr, int f=0);
};

struct compare
{
    bool operator()(const PayloadBlock *l, const PayloadBlock *r) {
        return l->payload->get_time_stamp() > r->payload->get_time_stamp();
    }
};

#endif
