#include "ClientImpl.hpp"
#include "packets/Login.hpp"
#include "packets/Message.hpp"

#include "ChatGUI.hpp"
#include <deque>
#include <iostream>
#include <string>
int main()
{
    asio::io_context context;
    auto workGuard = asio::make_work_guard(context);
    std::thread contextThread([&context]() {
        context.run();
        });
    ChatClient client(context);
    client.connect("127.0.0.1", 7777);
    while (!client.connected())
    {
        std::this_thread::yield();
    }
    ChatGUIManager manager(&client);
    client.setPacketHandler([&manager](std::vector<std::uint8_t> data) {
        if (data[0] == 0x01)
        {
            if (data[1] == 0x01)
            {
                Login packet(std::move(data));
                if (packet.result == LOGIN_RESULT::SUCCESS)
                {
                    manager.setState(AppState::CHAT);
                }
                else
                {
                    manager.setLoginError("User already logged in.");
                }
            }
            else if (data[1] == 0x02)
            {
                Message packet(std::move(data));
                manager.addChatMessage(packet.name, packet.message);
            }
        }
        });

    while (client.connected())
    {
        std::this_thread::yield();
    }
    contextThread.join();
}
