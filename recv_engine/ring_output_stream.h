
#ifndef _RING_OUTPUT_STREAM_H
#define _RING_OUTPUT_STREAM_H

#include <parquet/util/memory.h>

#include "configs.h"


class RingPool : public arrow::MemoryPool
{
    public:
        RingPool();

        ~RingPool();

        arrow::Status Allocate(int64_t size, uint8_t **out) override;

        void Free(uint8_t* p, int64_t size) override;

        arrow::Status Reallocate(int64_t old_size, int64_t new_size, uint8_t** out) { return arrow::Status::OK(); }

        int64_t bytes_allocated() const { return _bytes_allocated; }

    private:
        int64_t _bytes_allocated;
};

class RingBuffer : public arrow::MutableBuffer
{
    public:
        explicit RingBuffer(arrow::MemoryPool* pool);

        ~RingBuffer();

        arrow::Status Reserve(int64_t initial_capacity);

    private:
        arrow::MemoryPool *pool_;
};

class RingOutputStream : public OutputStream 
{
    public:
        explicit RingOutputStream(arrow::MemoryPool *pool, 
                    int64_t initial_capacity=MAX_PARQUET_FILE_SIZE);

        virtual ~RingOutputStream();

        virtual int64_t Tell() { return size_; }

        virtual void Close() {}

        virtual void Write(const uint8_t* data, int64_t length);

        std::shared_ptr<Buffer> GetBuffer();

        void Clear() { size_ = 0; }

        const uint8_t *Data() { return buffer_->mutable_data(); }

    private:
        uint8_t* Head() { return buffer_->mutable_data() + size_; }

        std::shared_ptr<RingBuffer> buffer_; 

        int64_t size_;

        int64_t capacity_;

        void operator=(const RingOutputStream&) = delete;

        RingOutputStream(const RingOutputStream&) = delete;
};

#endif // _RING_OUTPUT_STREAM_H
