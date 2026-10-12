#include <iostream>
#include <string>
#include <vector>
#include <httplib.h>

//Information of Requested Changes:
/*struct AccessRequest {
    std::string username;
    std::string role;
    bool device_trust;
    std::string resource;
    std::string action;
};

//Access Decision Function
struct AccessDecision {
    std::string effect;
    std::string reason_code;
};

AccessDecision access_decision(const AccessRequest& request) {
    if(!request.device_trust) {
        return {"DENY", "DEVICE_NOT_TRUSTED"};
    }
    if(request.resource == "public" && request.action == "read") {
        return {"PERMIT", "PUBLIC_READ_ALLOWED"};
    }
    if(request.resource == "internal" && request.role=="manager" ||
        request.role=="admin" && request.action == "read") {
        return {"PERMIT", "INTERNAL_READ_ALLOWED"};
    }
    if(request.resource == "secret" && request.role=="employee" && request.action == "read") {
        return {"STEP_UP", "SECRET_READ_STEP_UP_REQUIRED"};
    }
    if(request.resource == "secret" && request.role=="admin" && request.action == "read") {
        return {"PERMIT", "SECRET_ADMIN_READ_ALLOWED"};
    }
    return {"DENY", "ACCESS_DENIED"};
}

void print_access_decision(const AccessRequest& request ,const AccessDecision& decision) {
    std::cout << "USER: " << request.username << std::endl;
    std::cout << "ROLE: " << request.role << std::endl;
    std::cout << "DEVICE TRUST: " << (request.device_trust ? "TRUE" : "FALSE") << std::endl;
    std::cout << "RESOURCE: " << request.resource << std::endl;
    std::cout << "ACTION: " << request.action << std::endl;
    std::cout << "Access Decision: " << decision.effect << std::endl;
    std::cout << "Reason Code: " << decision.reason_code << std::endl;
}

int main(){
    std::vector<AccessRequest> requests = {
        {"alice", "employee", true, "public", "read"},
        {"bob", "manager", true, "internal", "read"},
        {"charlie", "employee", true, "secret", "read"},
        {"dave", "admin", true, "secret", "read"},
        {"eve", "employee", false, "public", "read"}
    };

} */

int main(){
    httplib::Server server;

    server.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content(R"({service: "ztas", status: "UP"})", "application/json");
    });

    std::cout << "ZTAS service is running on http://localhost:8020" << std::endl;

    if(!server.listen("0.0.0.0", 8020)){
        std::cerr << "Cannot start ZTAS on port 8020 " << std::endl;
        return 1;
    }
    return 0;
}