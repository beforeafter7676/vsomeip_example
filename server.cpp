#include <vsomeip/vsomeip.hpp>
#include <iostream>

#define SERVICE_ID 0x1234
#define INSTANCE_ID 0x5678
#define METHOD_ID 0x0421

class Server {
public:
    Server() {
        app_ = vsomeip::runtime::get()->create_application("ServiceApp");
    }

    void init() {
        app_->init();
        app_->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID,
            std::bind(&Server::on_message, this, std::placeholders::_1));
        app_->offer_service(SERVICE_ID, INSTANCE_ID);
    }

    void start() {
        app_->start();
    }

    void on_message(const std::shared_ptr<vsomeip::message> &request) {
        auto payload = request->get_payload();
        std::vector<vsomeip::byte_t> data = payload->get_data();

        std::cout << "[Server] Received request!" << std::endl;

        auto response = app_->create_response(request);
        std::shared_ptr<vsomeip::payload> resp_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> resp_data = {0xDE, 0xAD, 0xBE, 0xEF};
        resp_payload->set_data(resp_data);
        response->set_payload(resp_payload);

        app_->send(response);
    }

private:
    std::shared_ptr<vsomeip::application> app_;
};

int main() {
    Server server;
    server.init();
    server.start();
}
