#pragma once
#ifndef ___Class_RingBuffer
#define ___Class_RingBuffer

#include <vector>

using namespace std;

template <typename T>
class RingBuffer {
private:
	int buffer_size, push_offset, read_offset;
public:
	vector<T> buffer;

	RingBuffer(int buffer_size = 32) :buffer_size(buffer_size), push_offset(0) {
		this->buffer.resize(buffer_size);
	}

	void forward_offset() {
		this->read_offset = this->push_offset++;
	}

	T& get_push_position() {
		if (this->push_offset == this->buffer_size) {
			this->push_offset = 0;
		}
		return this->buffer[this->push_offset];
	}

	T& get_read_position() {
		return this->buffer[this->read_offset];
	}
};

#endif