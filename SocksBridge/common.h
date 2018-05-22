#pragma once
#include<string>
#include<memory>
#include<boost/system/error_code.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio/ip/address.hpp>
#include<boost/asio/io_service.hpp>
#include<boost/shared_array.hpp>
namespace seraphim {
using std::string;
using std::shared_ptr;
using boost::system::error_code;
using boost::shared_array;
using endpoint = boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> ;
using socket = boost::asio::ip::tcp::socket;


class ByteOStream {
private:
	uint8_t * buf_;
	const size_t  max_size_;
	size_t	offset_;
public:
	ByteOStream(uint8_t* buf, size_t len) :buf_(buf), max_size_(len), offset_(0) {

	}
	template <typename T>
	void put(T t) {
		size_t cbType = sizeof(T);
		//assert((cbType + offset_) <= max_size_);
		if ((cbType + offset_) > max_size_) {
			//LaOG_ERROR<<
		}
		uint8_t* src = reinterpret_cast<uint8_t*>(&t);
		memcpy(buf_ + offset_, src, cbType);
		offset_ += cbType;
	}
	template<typename T>
	void putN(T t) {
		size_t cbType = sizeof(T);
		//assert((cbType + offset_) <= max_size_);
		uint8_t* src = reinterpret_cast<uint8_t*>(&t);
		for (int i = static_cast<int>(cbType - 1); i >= 0; i--) {
			buf_[offset_++] = src[i];
		}
	}
	void put_arry(uint8_t* src, size_t len) {
		//assert(len + offset_ <= max_size_);
		memcpy(&buf_[offset_], src, len);
		offset_ += len;

	}
	//template<typename T>
	void put_arry(shared_array<uint8_t> buf, size_t len) {
		put_arry(buf.get(), len);
		offset_ += len;
	}
	void reset() {
		memset(buf_, 0, max_size_);
		offset_ = 0;
	}
};

class ByteIStream {
private:
	uint8_t * buf_;
	size_t   offset_;
	size_t max_len;
public:
	ByteIStream(uint8_t* buf, size_t len) :buf_(buf), offset_(0), max_len(len)
	{
	}

	template<typename T>
	T get() {
		T t;
		size_t cbType = sizeof(T);
		//assert((max_len - offset_ - cbType) >= 0);
		uint8_t*  p = reinterpret_cast<uint8_t*>(&t);
		for (size_t i = 0; i < cbType; i++) {
			p[i] = buf_[offset_++];
		}
		return t;
	}
	template<typename T>
	T getN() {
		T t;
		size_t cbType = sizeof(T);
		//assert((max_len - offset_ - cbType) >= 0);
		uint8_t*  p = reinterpret_cast<uint8_t*>(&t);
		;
		for (int i = static_cast<int>(cbType - 1); i >= 0; i--) {
			p[i] = buf_[offset_++];
		}
		return t;
	}

	template<typename T>
	void get_array(T* dst, size_t len) {
		memcpy(&buf_[offset_], dst, len);
		offset_ += len;
	}
	void reset() {
		memset(buf_, 0, max_len);
		offset_ = 0;
	}
};



};
