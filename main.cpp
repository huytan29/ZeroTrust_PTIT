#include <iostream>
#include <string>
#include <vector>


//Thong tin cua mot yeu cau truy cap
struct AccessRequest {
    std::string username;
    std::string role;
    bool devce_trust;
    std::string resource;
    std::string action;
};

//Ket qua cua viec danh gia yeu cau truy cap
struct AccessDecision {
    std::string effect; // "allow" hoac "deny"
    std::string reason_code;
};


//Day la Rule giai doan mo phong
//O giai doan sau, phan nay se duoc thay the bang viec goi OPA
AccessDecision evaluate_access(const AccessRequest& request) {
    // Zero Trust: thiet bi khong tin cay thi tu choi truoc
    if (!request.devce_trust){
        return {"DENY", "DEVICE_NOT_TRUSTED"};
    }
    //TAI LIEU CONG KHAI: MOI NGUOI DUNG HOP LE CO THE DOC
    if (request.resource == "public" && request.action == "read"){
        return {"PERMIT", "PUBLIC_READ_ALLOWED"};
    }
    //Tai lieu noi bo: chi nhung nguoi dung co vai tro quan ly hoac admin moi duoc doc
    if (request.resource == "internal" && 
    (request.role == "mamnage" || request.role == "admin") && request.action == "read"){
        return {"PERMIT", "INTERNAL_READ_ALLOWED"};
    };
    //TAI LIEU MAT: NHAN VIEN PHAI XAC THUC THEM
    if (request.resource == "secret" && 
    request.role == "employee" && 
    request.action == "read"){
        return {"STEP_UP", "MFA_REQUIRED_FOR_SECRET"};
    }
    //MOI TRUONG HOP KHONG KHOP RULE DEU BI TU CHOI
    return {"DENY", "NO_MATCHING_RULE"};

}

void print_result(const AccessRequest& request, const AccessDecision& decision){
    std::cout << "User: " << request.username << std::endl;
    std::cout << "Role: " << request.role << std::endl;
    std::cout << "Device Trust: " << (request.devce_trust ? "true" : "false") << std::endl;
    std::cout << "Resource: " << request.resource << std::endl;
    std::cout << "Action: " << request.action << std::endl;
    std::cout << "Decision: " << decision.effect << std::endl;
    std::cout << "Reason: " << decision.reason_code << std::endl;   
    std::cout << "-----------------------------" << std::endl;
}

int main(){
    std::vector<AccessRequest> requests = {
        {"alice", "employee", true, "public", "read"},
        {"bob", "employee", true, "internal", "read"},
        {"carol", "admin", true, "internal", "read"},
        {"dave", "employee", false, "public", "read"},
        {"eve", "employee", true, "secret", "read"},
        {"frank", "manager", true, "secret", "read"}
    };
    for (const auto& request : requests) {
        auto decision = evaluate_access(request);
        print_result(request, decision);
    }
    return 0;
}