#include <vsomeip/vsomeip.hpp>
#include <iostream>

#define SERVICE_ID 0x1234
#define INSTANCE_ID 0x5678
#define METHOD_ID 0x0421

class Client {
public:
    Client() {
        app_ = vsomeip::runtime::get()->create_application("ClientApp");
    }

    void init() {
        app_->init();
        app_->register_state_handler(std::bind(&Client::on_state, this, std::placeholders::_1));
        app_->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID,
            std::bind(&Client::on_message, this, std::placeholders::_1));
    }

    void start() {
        app_->start();
    }

    void on_state(vsomeip::state_type_e state) {
        if (state == vsomeip::state_type_e::ST_REGISTERED) {
            app_->request_service(SERVICE_ID, INSTANCE_ID);
            std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
            request->set_service(SERVICE_ID);
            request->set_instance(INSTANCE_ID);
            request->set_method(METHOD_ID);
            std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
            std::vector<vsomeip::byte_t> data = {0x01, 0x02, 0x03};
            payload->set_data(data);
            request->set_payload(payload);

            std::cout << "[Client] Sending request..." << std::endl;
            app_->send(request);
        }
    }

    void on_message(const std::shared_ptr<vsomeip::message> &response) {
        auto payload = response->get_payload();
        std::vector<vsomeip::byte_t> data = payload->get_data();
        std::cout << "[Client] Received response: ";
        for (auto b : data)
            std::cout << std::hex << (int)b << " ";
        std::cout << std::dec << std::endl;
    }

private:
    std::shared_ptr<vsomeip::application> app_;
};

int main() {
    Client client;
    client.init();
    client.start();
}
