package zerotrust

import rego.v1

# Mac dinh tu choi
default decision := "DENY"

# Chi cho phep khi cac dieu kien ben duoi deu dung
decision := "PERMIT" if {
    some role in input.subject.roles
    role in {"employee", "manager"}

    input.device.trust == "trusted"

    input.resource.sensitivity == "internal"

    input.action == "read"
}