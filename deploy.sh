#!/bin/bash
set -euo pipefail       # an toàn: dừng khi lỗi + biến chưa set + lỗi trong pipe

APP_NAME="my-app"
VERSION="${1:-latest}"

echo "==> Build $APP_NAME:$VERSION"
docker build -t "$APP_NAME:$VERSION" .

echo "==> Dừng container cũ"
docker rm -f "$APP_NAME" 2>/dev/null || true

echo "==> Chạy container mới"
docker run -d -p 3000:3000 --name "$APP_NAME" "$APP_NAME:$VERSION"

echo "==> Dọn image cũ không dùng"
docker image prune -f

echo "==> Kiểm tra trạng thái"
if docker ps --filter "name=$APP_NAME" --filter "status=running" | grep -q "$APP_NAME"; then
    echo "✓ Deploy thành công! $APP_NAME đang chạy."
else
    echo "✗ Deploy thất bại! Kiểm tra logs:"
    docker logs "$APP_NAME"
    exit 1
fi
