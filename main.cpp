#include <httplib.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

using json = nlohmann::json;

std::string get_env_or_default(const char* name, const std::string& fallback) {
    const char* value = std::getenv(name);
    return value != nullptr ? value : fallback;
}

void send_json(httplib::Response& response, int status, const json& body) {
    response.status = status;
    response.set_content(body.dump(), "application/json");
}

std::optional<json> query_opa(const std::string& opa_host, const json& input) {
    httplib::Client client(opa_host, 8181);

    client.set_connection_timeout(2, 0);
    client.set_read_timeout(3, 0);

    json request_body = {
        {"input", input}
    };

    auto result = client.Post(
        "/v1/data/zerotrust/authz/decision",
        request_body.dump(),
        "application/json"
    );

    if (!result || result->status != 200) {
        return std::nullopt;
    }

    try {
        json payload = json::parse(result->body);

        if (!payload.contains("result")) {
            return std::nullopt;
        }

        return payload["result"];
    } catch (const json::parse_error&) {
        return std::nullopt;
    }
}

void handle_document_request(
    const httplib::Request& request,
    httplib::Response& response,
    const std::string& opa_host,
    const std::string& resource_id,
    const std::string& sensitivity,
    const std::string& document_content
) {
    if (!request.has_header("X-Demo-Role") ||
        !request.has_header("X-Device-ID")) {
        send_json(response, 400, {
            {"error", "MISSING_ACCESS_CONTEXT"},
            {"message", "X-Demo-Role va X-Device-ID la bat buoc"}
        });
        return;
    }

    json input = {
        {"subject", {
            {"username", "demo-user"},
            {"role", request.get_header_value("X-Demo-Role")}
        }},
        {"device", {
            {"id", request.get_header_value("X-Device-ID")}
        }},
        {"resource", {
            {"id", resource_id},
            {"sensitivity", sensitivity}
        }},
        {"action", "read"}
    };

    auto decision = query_opa(opa_host, input);

    if (!decision.has_value()) {
        send_json(response, 502, {
            {"error", "OPA_UNAVAILABLE"},
            {"message", "Khong the lay quyet dinh tu OPA"}
        });
        return;
    }

    std::string effect = decision->value("effect", "DENY");

    json result = *decision;
    result["resource_id"] = resource_id;

    if (effect == "PERMIT") {
        result["content"] = document_content;
        send_json(response, 200, result);
        return;
    }

    if (effect == "STEP_UP") {
        send_json(response, 403, result);
        return;
    }

    send_json(response, 403, result);
}

int main() {
    const std::string opa_host = get_env_or_default("OPA_HOST", "localhost");

    httplib::Server server;

    server.Get("/health", [&opa_host](
        const httplib::Request&,
        httplib::Response& response
    ) {
        send_json(response, 200, {
            {"service", "ztas"},
            {"status", "UP"},
            {"opa_host", opa_host}
        });
    });

    server.Get("/documents/public", [&opa_host](
        const httplib::Request& request,
        httplib::Response& response
    ) {
        handle_document_request(
            request, response, opa_host,
            "public_document", "public",
            "Noi dung tai lieu cong khai"
        );
    });

    server.Get("/documents/internal", [&opa_host](
        const httplib::Request& request,
        httplib::Response& response
    ) {
        handle_document_request(
            request, response, opa_host,
            "internal_document", "internal",
            "Noi dung tai lieu noi bo"
        );
    });

    server.Get("/documents/secret", [&opa_host](
        const httplib::Request& request,
        httplib::Response& response
    ) {
        handle_document_request(
            request, response, opa_host,
            "secret_document", "secret",
            "Noi dung tai lieu mat gia lap"
        );
    });

    std::cout << "ZTAS is running at http://localhost:8020" << std::endl;

    if (!server.listen("0.0.0.0", 8020)) {
        std::cerr << "Cannot start ZTAS on port 8020" << std::endl;
        return 1;
    }

    return 0;
}