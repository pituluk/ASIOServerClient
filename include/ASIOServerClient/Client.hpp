#ifndef ASIOSERVERCLIENT_CLIENT_HPP
#define ASIOSERVERCLIENT_CLIENT_HPP
#include <algorithm>
#include <array>
#include <asio.hpp>
#ifdef ASIOSERVERCLIENT_HAVE_OPENSSL
#include <asio/ssl.hpp>
#endif
#include <atomic>
#include <concepts>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <variant>
#include <vector>

class TCPClient {
public:
    using PlainSocket = asio::ip::tcp::socket;
#ifdef ASIOSERVERCLIENT_HAVE_OPENSSL
    using SslSocket = asio::ssl::stream<asio::ip::tcp::socket>;
    using SocketVariant = std::variant<PlainSocket, SslSocket>;
#else
    using SocketVariant = std::variant<PlainSocket>;
#endif
    asio::io_context::strand strand;

    TCPClient(asio::io_context& io_context);
#ifdef ASIOSERVERCLIENT_HAVE_OPENSSL
    TCPClient(asio::io_context& io_context, asio::ssl::context& ssl_context);
#endif

    virtual ~TCPClient() = default;
    void connect(const std::string& host, std::uint16_t port);
    bool connected() { return connected_ && !markedToDisconnect; }
    void disconnect();
    void send(std::vector<std::uint8_t> data);
protected:
    virtual void onConnected() {}
    virtual void onDisconnect() {}
    virtual void onData(const std::vector<std::uint8_t>& data) {}
    virtual void onError(const std::error_code& ec, const std::string_view message) {}
private:
    std::atomic<bool> connected_ = false;
    std::atomic<bool> markedToDisconnect = false;
    asio::ip::tcp::resolver resolver_;
    bool use_ssl_ = false;
    std::string host_;
    uint16_t port_ = 0;
    SocketVariant socket_;
    std::array<std::uint8_t, 65535> read_buffer_;
    std::deque<std::vector<std::uint8_t>> write_queue_;

    void do_read();
    void do_write();
};

#endif // ASIOSERVERCLIENT_CLIENT_HPP
