
#include "log.h"
#include "ring_output_stream.h"


/* ------------RingPool---------------- */
RingPool::RingPool() {}

RingPool::~RingPool() {}

arrow::Status RingPool::Allocate(int64_t size, uint8_t **out) 
{
    uint8_t *bucket = nullptr;
    while (nullptr == (bucket = GetConfigs().get_parquet_mem_resource())) {
        LOG_DEBUG("[parquet] wait get_parquet_mem_resource");
        GetConfigs().show_all_ring_stat();
        xdf::mrmw_ring_wait_event(GetConfigs().parquet_mem_resource_ring, 3);
    }
    *out = bucket;
    _bytes_allocated = size;
    return arrow::Status::OK();
}

void RingPool::Free(uint8_t* p, int64_t size) 
{
    while (!GetConfigs().put_parquet_mem_resource(p)) {
        LOG_DEBUG("[hdfs] wait put_parquet_mem_resource");
        GetConfigs().show_all_ring_stat();
        xdf::mrmw_ring_wait_event(GetConfigs().parquet_mem_resource_ring, 3);
    }
    xdf::mrmw_ring_notify_all(GetConfigs().parquet_mem_resource_ring);
    _bytes_allocated = 0;
}

/* ----------------RingBuffer----------------- */
RingBuffer::RingBuffer(arrow::MemoryPool* pool)
    : MutableBuffer(nullptr, 0), pool_(pool) {}

RingBuffer::~RingBuffer() {
    pool_->Free(mutable_data_, size_);
    mutable_data_ = nullptr;
}

arrow::Status RingBuffer::Reserve(int64_t initial_capacity) {
    if (!mutable_data_) {
        size_ = initial_capacity;
        pool_->Allocate(size_, &mutable_data_);
    }
    return arrow::Status::OK();
}

/* ----------------RingOutputStream-------------- */
RingOutputStream::RingOutputStream(arrow::MemoryPool *pool, int64_t initial_capacity)
    : size_(0), capacity_(initial_capacity) {
    buffer_ = std::make_shared<RingBuffer>(pool);
    buffer_->Reserve(capacity_);
}

RingOutputStream::~RingOutputStream() {}

void RingOutputStream::Write(const uint8_t* data, int64_t length) {
    memcpy(Head(), data, length);
    size_ += length;
}

std::shared_ptr<Buffer> RingOutputStream::GetBuffer() {
    std::shared_ptr<Buffer> result = buffer_;
    buffer_ = nullptr;
    return result;
}
