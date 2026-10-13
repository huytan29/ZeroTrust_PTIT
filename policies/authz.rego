package zerotrust.authz

default decision := {
    "effect": "DENY",
    "reason_code": "POLICY_NOT_MATCHED",
    "policy_version": "zt-demo-1.0.0"
}

# Public: thiết bị trusted được đọc.
decision := {
    "effect": "PERMIT",
    "reason_code": "PUBLIC_READ_ALLOWED",
    "policy_version": "zt-demo-1.0.0"
} if {
    input.action == "read"
    input.resource.sensitivity == "public"
    device_trusted
}

# Internal: chỉ manager hoặc admin trên thiết bị trusted.
decision := {
    "effect": "PERMIT",
    "reason_code": "INTERNAL_READ_ALLOWED",
    "policy_version": "zt-demo-1.0.0"
} if {
    input.action == "read"
    input.resource.sensitivity == "internal"
    input.subject.role in {"manager", "admin"}
    device_trusted
}

# Secret: employee phải xác thực thêm.
decision := {
    "effect": "STEP_UP",
    "reason_code": "MFA_REQUIRED_FOR_SECRET",
    "policy_version": "zt-demo-1.0.0"
} if {
    input.action == "read"
    input.resource.sensitivity == "secret"
    input.subject.role == "employee"
    device_trusted
}

# Secret: admin được đọc trên thiết bị trusted.
decision := {
    "effect": "PERMIT",
    "reason_code": "ADMIN_SECRET_READ_ALLOWED",
    "policy_version": "zt-demo-1.0.0"
} if {
    input.action == "read"
    input.resource.sensitivity == "secret"
    input.subject.role == "admin"
    device_trusted
}

device_trusted if {
    data.devices[input.device.id].trust == "trusted"
}