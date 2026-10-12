Only fix:
- Action: read
- Resource: public, internal, secret
- Roles: employee, manager, admin
- Device states: trusted, untrusted, unknow
- Decisions: PERMIT, DENY, STEP_UP

Pull Request to Access for one 

Keycloak get API for user


Script an toàn "set -e" và kiểm tra

set -e # Dừng ngay nếu bất kỳ lệnh nào lỗi (không chạy tiếp)
set -u #báo lỗi nếu dùng biến chưa định nghĩa

#Kiểm tra điều kiện trước khi chạy
if ! docker info >/dev/null 2>&1; then
    echo "Lỗi: Docker daemon không chạy!"
    exit 1
fi

echo "Đang Deploy..."
docker build -t my-app:1.0 .
